#include "ddknd/graphics/gfx_asset_loader.h"

#include <cassert>
#include <optional>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

#include "ddknd/asset/asset_manager.h"
#include "ddknd/asset/sub_asset_key.h"

#include "ddknd/graphics/renderer_backend.h"
#include "internal/asset/shader_descriptor_parser.h"
#include "internal/graphics/model_loader/skeleton_builder.h"
#include "internal/graphics/model_importer/model_import_types.h"
#include "internal/graphics/model_importer/stb_image_decoder.h"
#include "internal/io/io.h"

#include "internal/graphics/model_importer/glb_importer.h"

#include "internal/graphics/font_importer/stb_font_importer.h"

#include "ddknd/math/math.h"

#include <spdlog/spdlog.h>

namespace
{
    // internal
    using ImportModelData = ddknd::graphics::internal::types::ModelImportData;

    using ImportChannelType = ddknd::graphics::internal::types::ChannelType;

    using ModelRenderResource = ddknd::graphics::types::ModelRenderResource;
    using SkeletonResource = ddknd::animation::types::SkeletonResource;
    using AnimationChannel = ddknd::animation::types::AnimationChannel;
    using Bone = ddknd::animation::types::Bone;

    using TextureResource = ddknd::graphics::types::TextureResource;
    using MaterialResource = ddknd::graphics::types::MaterialResource;

    template <typename Tag>
    using AssetID = ddknd::asset::AssetID<Tag>;
    using AssetManager = ddknd::asset::AssetManager;

    using GraphicsAssetStore = ddknd::graphics::GraphicsAssetStore;
    using IRendererBackend = ddknd::graphics::IRendererBackend;
    using PrimitiveKey = ddknd::graphics::types::PrimitiveKey;
    using TextureID = AssetID<ddknd::asset::tag::TextureTag>;

    using AnimationAssetStore = ddknd::animation::AnimationAssetStore;
    using AnimationClipResource = ddknd::animation::types::AnimationClipResource;
    using ChannelTarget = ddknd::animation::types::ChannelTarget;

    struct ModelBuildResult
    {
        ModelRenderResource model;
        std::unordered_map<int, int> nodeToBone;
    };

    // Generic conversion helpers
    std::uint32_t ResolveMaterialIndexOrDefault(int importMaterialIndex, std::size_t materialCount)
    {
        if (materialCount == 0)
        {
            return 0;
        }

        if (importMaterialIndex < 0)
        {
            return 0;
        }

        const auto index = static_cast<std::size_t>(importMaterialIndex);

        if (index >= materialCount)
            return 0;

        return static_cast<std::uint32_t>(index);
    }

    std::optional<TextureID> ResolveTextureID(std::span<const TextureID> textureMap,
                                              std::optional<std::uint32_t> importTextureIndex)
    {
        if (!importTextureIndex)
            return std::nullopt;

        const auto index = *importTextureIndex;

        if (index >= textureMap.size())
            return std::nullopt;

        const TextureID id = textureMap[index];

        if (!id.IsValid())
            return std::nullopt;

        return id;
    }

    // Texture and material conversion
    ::ddknd::graphics::types::SamplerDesc BuildSamplerDescOrDefault(
        const ImportModelData& import, const ddknd::graphics::internal::types::ImportTexture& tex)
    {
        using namespace ::ddknd::graphics::types;
        SamplerDesc desc{};

        if (!tex.sampler)
            return desc;

        const auto samplerIndex = *tex.sampler;
        if (samplerIndex >= import.samplers.size())
        {
            spdlog::warn("[BuildSamplerDescOrDefault] Invalid sampler index: {}. Using defaults.", samplerIndex);
            return desc;
        }

        const auto& src = import.samplers[samplerIndex];

        if (src.minFilter)
            desc.minFilter = *src.minFilter;
        if (src.magFilter)
            desc.magFilter = *src.magFilter;

        desc.wrapS = src.wrapS;
        desc.wrapT = src.wrapT;

        return desc;
    }

    std::optional<TextureResource> BuildTextureResourceFromImport(const ImportModelData& import,
                                                                  std::uint32_t textureIndex,
                                                                  ddknd::graphics::IRendererBackend& backend)
    {

        if (textureIndex >= import.textures.size())
            return std::nullopt;
        const auto& importTexture = import.textures[textureIndex];

        if (!importTexture.source)
            return std::nullopt;
        const auto imageIndex = *importTexture.source;

        if (imageIndex >= import.images.size())
            return std::nullopt;
        const auto& image = import.images[imageIndex];

        auto decoded = ::ddknd::graphics::internal::DecodeImageRGBA8(image.encodedBytes);
        if (!decoded)
            return std::nullopt;

        const auto samplerDesc = BuildSamplerDescOrDefault(import, importTexture);

        ::ddknd::graphics::types::Texture2DCreateDesc desc{};
        desc.width = decoded->width;
        desc.height = decoded->height;
        desc.format = ::ddknd::graphics::types::TextureFormat::RGBA8;
        desc.pixels = std::span<const std::uint8_t>(decoded->pixels.data(), decoded->pixels.size());
        desc.sampler = samplerDesc;
        desc.generateMipmap = true;

        const auto gpuTexture = backend.CreateTexture2D(desc);
        if (!gpuTexture.IsValid())
        {
            return std::nullopt;
        }

        TextureResource out{};
        out.gpuTexture = gpuTexture;
        out.width = desc.width;
        out.height = desc.height;
        out.channels = decoded->channels;
        out.format = desc.format;

        return out;
    }

    MaterialResource BuildMaterialResourceFromImport(const ImportModelData& import, std::uint32_t materialIndex,
                                                     std::span<const TextureID> textureMap)
    {
        MaterialResource out{};
        if (materialIndex >= import.materials.size())
            return out;
        const auto& importMaterial = import.materials[materialIndex];
        const auto& pbr = importMaterial.pbrMetallicRoughness;

        out.baseColorFactor = pbr.baseColorFactor;
        out.metallicFactor = pbr.metallicFactor;
        out.roughnessFactor = pbr.roughnessFactor;
        out.emissiveFactor = importMaterial.emissiveFactor;
        out.alphaMode = importMaterial.alphaMode;
        out.alphaCutoff = importMaterial.alphaCutoff;
        out.doubleSided = importMaterial.doubleSided;

        // Resolve imported texture indices to runtime TextureIDs.
        const auto baseColorTexture = ResolveTextureID(textureMap, pbr.baseColorTexture.index);
        if (baseColorTexture)
        {
            out.baseColorTexture.texture = *baseColorTexture;
            out.baseColorTexture.texCoord = pbr.baseColorTexture.texCoord;
        }

        const auto metallicRoughnessTexture = ResolveTextureID(textureMap, pbr.metallicRoughnessTexture.index);
        if (metallicRoughnessTexture)
        {
            out.metallicRoughnessTexture.texture = *metallicRoughnessTexture;
            out.metallicRoughnessTexture.texCoord = pbr.metallicRoughnessTexture.texCoord;
        }

        const auto& normal = importMaterial.normalTexture;
        const auto normalTexture = ResolveTextureID(textureMap, normal.index);
        if (normalTexture)
        {
            out.normalTexture.texture = *normalTexture;
            out.normalTexture.texCoord = normal.texCoord;
            out.normalTexture.scale = normal.scale;
        }

        const auto& occlusion = importMaterial.occlusionTexture;
        const auto occlusionTexture = ResolveTextureID(textureMap, occlusion.index);
        if (occlusionTexture)
        {
            out.occlusionTexture.texture = *occlusionTexture;
            out.occlusionTexture.texCoord = occlusion.texCoord;
            out.occlusionTexture.strength = occlusion.strength;
        }

        const auto& emissive = importMaterial.emissiveTexture;
        const auto emissiveTexture = ResolveTextureID(textureMap, emissive.index);
        if (emissiveTexture)
        {
            out.emissiveTexture.texture = *emissiveTexture;
            out.emissiveTexture.texCoord = emissive.texCoord;
        }

        return out;
    }


    // Model conversion
    // Converts one imported glTF scene into runtime model resources.
    // glTF node indices are not assumed to follow hierarchy order.
    std::optional<ModelBuildResult> BuildModelRenderResource(const ImportModelData& import, int sceneIndex,
                                                             IRendererBackend& backend,
                                                             std::span<const TextureID> textureMap,
                                                             const std::string& vpath)
    {

        using PrimitiveResource = ::ddknd::graphics::types::PrimitiveResource;

        ModelBuildResult out;

        const auto& scenes = import.scenes;
        const auto& nodes = import.nodes;
        const auto& meshes = import.meshes;
        const auto& primitives = import.primitives;

        if (sceneIndex < 0 || scenes.size() <= static_cast<std::size_t>(sceneIndex))
        {
            spdlog::error("[BuildModelRenderResource]: Invalid Scene Index {}", sceneIndex);
            return std::nullopt;
        }
        out.model.sourceScene = sceneIndex;

        const auto& scene = scenes[sceneIndex];

        out.model.materials.resize(import.materials.size());

        for (std::size_t i = 0; i < import.materials.size(); ++i)
        {
            out.model.materials[i] = BuildMaterialResourceFromImport(import, static_cast<std::uint32_t>(i), textureMap);
        }

        // fallback
        if (out.model.materials.empty())
        {
            out.model.materials.push_back(MaterialResource{});
        }

        std::vector<int> roots = scene.rootNodes;
        std::vector<bool> visited(nodes.size(), false);
        std::vector<int> nodeInScene;

        // Traverse from the scene roots to ensure that each parent node is
        // visited before its descendants.
        // Bone order is determined separately by skin.jointNodes
        // and is not guaranteed to be parent-first.
        while (!roots.empty())
        {
            int n = roots.back();
            roots.pop_back();

            if (n < 0 || static_cast<std::size_t>(n) >= nodes.size())
            {
                spdlog::warn("[BuildModelRenderResource] Invalid node index: {}", n);
                continue;
            }

            if (visited[n])
                continue;

            visited[n] = true;
            nodeInScene.push_back(n);

            for (int c : nodes[n].children)
            {
                roots.push_back(c);
            }
        }

        for (auto i : nodeInScene)
        {
            const auto& node = nodes[i];

            if (node.mesh < 0)
                continue;

            const auto& mesh = meshes[node.mesh];

            for (int primIndex : mesh.primitives)
            {
                const auto& prim = primitives[primIndex];

                PrimitiveKey key{};
                key.fileKey = vpath;
                key.primitiveIndex = primIndex;

                // register to GPU
                ddknd::graphics::types::PrimitiveCreateData create{};
                create.vertices = prim.vertices;
                create.indices = prim.indices;
                create.materialIndex = prim.material;
                auto gpuPrim = backend.CreateOrGetPrimitive(create, key);

                PrimitiveResource r{};
                r.primitive = gpuPrim;
                r.vertexCount = static_cast<std::uint32_t>(prim.vertices.size());
                r.indexCount = static_cast<std::uint32_t>(prim.indices.size());
                r.materialIndex = ResolveMaterialIndexOrDefault(prim.material, out.model.materials.size());

                out.model.primitives.push_back(r);
            }
        }

        for (int n : nodeInScene)
        {
            const auto skin = nodes[n].skin;
            if (skin >= 0)
            {
                //out.model.skeleton = BuildModelSkeletonResource(import, skin, out.nodeToBone);
                auto skeletonResult = ddknd::graphics::internal::BuildModelSkeletonResource(import,skin);
                out.model.skeleton = std::move(skeletonResult.skeleton);
                out.nodeToBone = std::move(skeletonResult.nodeToBone);
                break;
            }
        }

        return out;
    }

    std::vector<AssetID<::ddknd::asset::tag::AnimationClip>> RegisterAnimationClips(
        AssetManager& assets, const ImportModelData& imported, const std::string& vpath /*vpath: Model_Asset_Key*/)
    {
        std::vector<AssetID<::ddknd::asset::tag::AnimationClip>> out;
        for (std::size_t i = 0; i < imported.animations.size(); i++)
        {
            const auto& anim = imported.animations[i];

            if (anim.name.empty())
            {
                auto key = std::string(vpath) + "#anim/index/" + std::to_string(i);
                out.push_back(assets.GetOrCreate<::ddknd::asset::tag::AnimationClip>(key));
                continue;
            }

            auto key = ::ddknd::asset::MakeAnimationClipKey(vpath, anim.name);
            auto clipId = assets.GetOrCreate<::ddknd::asset::tag::AnimationClip>(key);

            out.push_back(clipId);
        }

        return out;
    }

    AnimationClipResource BuildAnimationClipResource(const ImportModelData& imported, const std::size_t animIndex,
                                                     const std::unordered_map<int, int>& nodeToBone)
    {
        const auto& anims = imported.animations;

        assert(anims.size() > animIndex);

        const auto& src = anims[animIndex];

        AnimationClipResource out;
        out.name = src.name;
        out.duration = src.duration;

        for (const auto& ch : src.channels)
        {
            auto it = nodeToBone.find(ch.targetNode);
            if (it == nodeToBone.end())
            {
                const auto& node = imported.nodes[ch.targetNode];
                spdlog::debug("[BuildAnimationClipResource] targetNode={} name={} parent={} type={}", ch.targetNode,
                              node.name, node.parent, static_cast<int>(ch.type));
                continue;
            }
            AnimationChannel dst{};
            dst.bone = it->second;

            switch (ch.type)
            {
            case ImportChannelType::T:
                dst.target = ChannelTarget::Translation;
                dst.times = ch.times;
                dst.vec3Values = ch.v3;
                break;

            case ImportChannelType::R:
                dst.target = ChannelTarget::Rotation;
                dst.times = ch.times;
                dst.quatValues = ch.vq;
                break;

            case ImportChannelType::S:
                dst.target = ChannelTarget::Scale;
                dst.times = ch.times;
                dst.vec3Values = ch.v3;
                break;

            default:
                continue;
            }

            out.channels.push_back(std::move(dst));
        }

        return out;
    }

} // namespace

namespace ddknd::graphics
{

    bool GraphicsAssetLoader::LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id)
    {
        auto vpath = assets.TryPathOf<ShaderID::tag_type>(id);
        if (!vpath)
        {
            spdlog::error("[LoadShader] Failed to retrieve vpath.");
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto descriptorPath = resolver_.TryResolve(*vpath);
        if (!descriptorPath)
        {
            spdlog::error("[LoadShader] "
                          "Failed to resolve shader descriptor path: {}",
                          *vpath);

            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        using namespace ::ddknd::io;
        auto descriptorText = ReadAllText(*descriptorPath);
        if (!descriptorText)
        {
            spdlog::error("[LoadShader] "
                          "Failed to read shader descriptor: {}",
                          descriptorPath->string());
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        using namespace ::ddknd::asset::parser;
        ParseError err;
        auto docOpt = ParseKeyValueText(*descriptorText, &err);
        if (!docOpt)
        {
            spdlog::error("[LoadShader] ParseKeyValueText failed line={} msg={}", err.line, err.msg);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto descOpt = BuildShaderDesc(*docOpt, &err);
        if (!descOpt)
        {
            spdlog::error("[LoadShader] BuildShaderDesc failed line={} msg={}", err.line, err.msg);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        spdlog::debug("[LoadShader] desc vertex={} fragment={}", descOpt->vertex_path, descOpt->fragment_path);

        auto vsAbs = resolver_.TryResolve(descOpt->vertex_path);
        if (!vsAbs)
        {
            spdlog::error("[LoadShader] resolve vertex failed: {}", descOpt->vertex_path);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto fsAbs = resolver_.TryResolve(descOpt->fragment_path);
        if (!fsAbs)
        {
            spdlog::error("[LoadShader] resolve fragment failed: {}", descOpt->fragment_path);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        spdlog::debug("[LoadShader] desc abs path = {}", descriptorPath->string());
        spdlog::debug("[LoadShader] vs abs path = {}", vsAbs->string());
        spdlog::debug("[LoadShader] fs abs path = {}", fsAbs->string());

        auto vsText = ::ddknd::io::ReadAllText(*vsAbs);
        if (!vsText)
        {
            spdlog::error("[LoadShader] ReadAllText vertex failed: {}", vsAbs->string());
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto fsText = ::ddknd::io::ReadAllText(*fsAbs);
        if (!fsText)
        {
            spdlog::error("[LoadShader] ReadAllText fragment failed: {}", fsAbs->string());
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto prog = backend_.CreateShaderProgram(*vsText, *fsText);
        if (!prog.IsValid())
        {
            spdlog::error("[LoadShader] CreateShaderProgram failed");
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        ::ddknd::graphics::types::ShaderResource res{};
        res.program = prog;

        store.SetLoaded(id, std::move(res));
        assets.SetState(id, ::ddknd::asset::AssetState::Loaded);

        return true;
    }

    bool GraphicsAssetLoader::LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore,
                                        AnimationAssetStore& animstore, ModelID id)
    {
        auto vpath = assets.TryPathOf<ModelID::tag_type>(id);
        if (!vpath)
        {
            spdlog::error("[LoadModel] Failed to retrieve vpath.");

            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto path = resolver_.TryResolve(*vpath);
        if (!path)
        {
            spdlog::error("[LoadModel] Failed to resolve path: {}", *vpath);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        const std::string abs = path->string();

        auto imported = ddknd::graphics::internal::ImportModel(abs);
        if (!imported)
        {
            spdlog::error("[LoadModel] Failed to import model: {}", abs);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        std::vector<TextureID> textureMap(imported->textures.size());

        for (std::size_t i = 0; i < imported->textures.size(); i++)
        {
            auto texture_resource = BuildTextureResourceFromImport(*imported, static_cast<std::uint32_t>(i), backend_);

            if (!texture_resource)
            {
                spdlog::warn("[LoadModel]: Failed to build texture resource. textureIndex={}", i);
                continue;
            }

            const std::string textureVpath = ::ddknd::asset::MakeTextureKey(*vpath, i);
            TextureID textureID = assets.GetOrCreate<TextureTag>(textureVpath);
            gfxstore.SetLoaded(textureID, std::move(*texture_resource));
            assets.SetState(textureID, ::ddknd::asset::AssetState::Loaded);

            textureMap[i] = textureID;
        }

        // Only the first imported scene is currently loaded.
        constexpr int sceneIndex = 0;

        auto buildResult = BuildModelRenderResource(*imported, sceneIndex, backend_, textureMap, std::string(*vpath));

        if (!buildResult)
        {
            spdlog::error("[LoadModel] "
                          "Failed to build runtime model resource: model={}, sceneIndex={}",
                          *vpath, sceneIndex);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }
        
        /**
        * test for joint order
        */
        // static int num_skeletons = 0;
        // if(buildResult->model.skeleton.has_value())
        // {
        //     // spdlog::info("here");
        //     spdlog::info("skeleton number={}",++num_skeletons);
        //     int i = 0;
        //     for(const auto& b : buildResult->model.skeleton->bones)
        //     {
        //         spdlog::info("index={}, parent={}", i, b.parent);
        //         if( i > 0)
        //         {
        //             assert(i > b.parent && "order is the matter");
        //         }
        //         i++;
        //     }
        // }
        // The animations within the GLB are treated as sub-assets and given asset IDs
        if (!imported->animations.empty() && !buildResult->nodeToBone.empty())
        {
            auto clipIds = RegisterAnimationClips(assets, *imported, std::string(*vpath));

            for (std::size_t i = 0; i < clipIds.size(); ++i)
            {
                auto animation = BuildAnimationClipResource(*imported, i, buildResult->nodeToBone);

                animstore.SetLoaded(clipIds[i], std::move(animation));
                assets.SetState(clipIds[i], ::ddknd::asset::AssetState::Loaded);
            }

            buildResult->model.clips = std::move(clipIds);
        }

        gfxstore.SetLoaded(id, std::move(buildResult->model));
        assets.SetState(id, ddknd::asset::AssetState::Loaded);

        return true;
    }

    bool GraphicsAssetLoader::LoadFont(AssetManager& assets, GraphicsAssetStore& store, FontID id)
    {
        constexpr float debugFontSize = 18.0f;

        if (store.TryGet(id))
            return true;

        auto vpath = assets.TryPathOf(id);
        if (!vpath)
        {
            spdlog::error("[LoadFont] Failed to retrieve asset path.");
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto path = resolver_.TryResolve(*vpath);
        if (!path)
        {
            spdlog::error("[LoadFont] Failed to resolve path: {}", *vpath);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        const std::string abs = path->string();

        // importer
        auto imported = ::ddknd::graphics::internal::ImportFont(abs, debugFontSize);
        if (!imported)
        {
            spdlog::error("[LoadFont] Failed to import font: {}", abs);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto tex = backend_.CreateTextureR8(imported->atlasWidth, imported->atlasHeight, imported->atlasBitmap);

        if (!tex.IsValid())
        {
            spdlog::error("[LoadFont] Failed to create atlas texture: {}", abs);
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        // create resource
        ::ddknd::graphics::types::FontResource fontRes;

        fontRes.atlas = tex;
        fontRes.atlasWidth = imported->atlasWidth;
        fontRes.atlasHeight = imported->atlasHeight;
        fontRes.firstCodepoint = imported->firstCodepoint;
        fontRes.glyphCount = imported->glyphCount;
        fontRes.pixelHeight = debugFontSize;

        fontRes.glyphs.resize(imported->glyphs.size());

        const float invW = 1.0f / static_cast<float>(imported->atlasWidth);
        const float invH = 1.0f / static_cast<float>(imported->atlasHeight);

        for (std::size_t i = 0; i < imported->glyphs.size(); i++)
        {
            const auto& src = imported->glyphs[i];
            auto& dst = fontRes.glyphs[i];

            dst.uv0 = {src.x0 * invW, src.y0 * invH};
            dst.uv1 = {src.x1 * invW, src.y1 * invH};
            dst.size = {src.x1 - src.x0, src.y1 - src.y0};
            dst.offset = {src.xoff, src.yoff};

            dst.advance = src.xadvance;
        }

        store.SetLoaded(id, std::move(fontRes));
        assets.SetState(id, ::ddknd::asset::AssetState::Loaded);
        return true;
    }

} // namespace ddknd::graphics