#include "ddknd/graphics/gfx_asset_loader.h"

#include <unordered_set>
#include <cstddef>
#include <cstdint>

#include "ddknd/asset/asset_manager.h"
#include "ddknd/asset/sub_asset_key.h"


#include "ddknd/graphics/model_data.h"
#include "ddknd/graphics/renderer.h"
#include "internal/asset/shader_descriptor_parser.h"
#include "internal/graphics/builder/backend_create_descriptor.h"
#include "internal/graphics/model_importer/model_import_types.h"
#include "internal/graphics/model_importer/stb_image_decoder.h"
#include "internal/io/io.h"


#include "internal/graphics/model_importer/glb_importer.h"

#include "internal/graphics/font_importer/stb_font_importer.h"

#include "ddknd/graphics/gfx_type.h"
#include "ddknd/math/math.h"

#include <spdlog/spdlog.h>
#include <string_view>

// builder declare
namespace
{
    using namespace ::ddknd;
    // internal
    using ImportModelData = ::ddknd::graphics::internal::types::ModelImportData;
    ;
    using ImportSkin = ::ddknd::graphics::internal::types::ImportSkin;
    using AnimationClipTag = ::ddknd::asset::tag::AnimationClip;
    using ImportChannelType = ddknd::graphics::internal::types::ChannelType;

    using ModelRenderResource = ::ddknd::graphics::types::ModelRenderResource;
    using SkeletonResource = ::ddknd::animation::types::SkeletonResource;
    using AnimationChannel = ::ddknd::animation::types::AnimationChannel;
    using Bone = ::ddknd::animation::types::Bone;

    using TextureResource = ::ddknd::graphics::types::TextureResource;
    using MaterialResource = ::ddknd::graphics::types::MaterialResource;

    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;
    using AssetManager = ::ddknd::asset::AssetManager;

    using GraphicsAssetStore = ::ddknd::graphics::GraphicsAssetStore;
    using RendererBackned = ::ddknd::graphics::IRendererBackend;
    using PrimitiveKey = ::ddknd::graphics::types::PrimitiveKey;
    using TextureID = AssetID<asset::tag::TextureTag>;

    using AnimationAssetStore = ::ddknd::animation::AnimationAssetStore;
    using AnimationClipResource = ::ddknd::animation::types::AnimationClipResource;
    using ChannelTarget = ::ddknd::animation::types::ChannelTarget;

    struct ModelBuildResult
    {
        ModelRenderResource model;
        std::optional<std::unordered_map<int, int>> nodeToBone;
    };

    // ModelRenderResource BuildModelRenderResource(ImportModelData& data, GraphicsAssetStore& gfxstore);
    ModelBuildResult BuildModelRenderResource(const ImportModelData& import, int sceneIndex, RendererBackned& backend,
                                              std::span<const TextureID> textureMap,const std::string& vpath /*vpath: Model_Asset_Key*/);
    SkeletonResource BuildModelSkeletonResource(const ImportModelData& import, int skinIndex,
                                                std::unordered_map<int, int>&);
    int FindParentBoneIndex(const ImportSkin& skin, int parentNode);

    // std::vector<math::Mat4f> BuildImportNodeGlobalMatrices(const ImportModelData& import);

    // Animation
    AnimationClipResource BuildAnimationClipResource(const ImportModelData&, const std::size_t,
                                                     const std::unordered_map<int, int>&);

    // temporaly
    std::vector<AssetID<::ddknd::asset::tag::AnimationClip>> RegisterAnimationClips(
        AssetManager&, const ImportModelData&, const std::string& vpath /*vpath: Model_Asset_Key*/);

    std::optional<TextureResource> BuildTextureResourceFromImport(const ImportModelData& import,
                                                                  std::uint32_t textureIndex,
                                                                  graphics::IRendererBackend& backend);
    MaterialResource BuildMaterialResourceFromImport(const ImportModelData& import, std::uint32_t materialIndex,
                                                     std::span<const TextureID> textureMap);
} // namespace

namespace ddknd::graphics
{

    bool GraphicsAssetLoader::LoadShader(AssetManager& assets, GraphicsAssetStore& store, ShaderID id)
    {
        auto vpath = assets.TryPathOf<ShaderID::tag_type>(id);
        if (!vpath)
            return false;

        auto abs = resolver_.TryResolve(*vpath);
        if (!abs)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        using namespace ::ddknd::io;
        auto textOpt = ReadAllText(*abs);
        if (!textOpt)
        {
            spdlog::error("ShaderLoader: ReadAllText failed: {}", abs->string());
            return false;
        }

        using namespace ::ddknd::asset::parser;
        ParseError err;
        auto docOpt = ParseKeyValueText(*textOpt, &err);
        if (!docOpt)
        {
            spdlog::error("ShaderLoader: ParseKeyValueText failed line={} msg={}", err.line, err.msg);
            return false;
        }

        auto descOpt = BuildShaderDesc(*docOpt, &err);
        if (!descOpt)
        {
            spdlog::error("ShaderLoader: BuildShaderDesc failed line={} msg={}", err.line, err.msg);
            return false;
        }

        spdlog::info("ShaderLoader: desc vertex={} fragment={}", descOpt->vertex_path, descOpt->fragment_path);

        auto vsAbs = resolver_.TryResolve(descOpt->vertex_path);
        if (!vsAbs)
        {
            spdlog::error("ShaderLoader: resolve vertex failed: {}", descOpt->vertex_path);
            return false;
        }

        auto fsAbs = resolver_.TryResolve(descOpt->fragment_path);
        if (!fsAbs)
        {
            spdlog::error("ShaderLoader: resolve fragment failed: {}", descOpt->fragment_path);
            return false;
        }

        spdlog::info("ShaderLoader: desc abs path = {}", abs->string());
        spdlog::info("ShaderLoader: vs abs path = {}", vsAbs->string());
        spdlog::info("ShaderLoader: fs abs path = {}", fsAbs->string());

        auto vsText = ::ddknd::io::ReadAllText(*vsAbs);
        if (!vsText)
        {
            spdlog::error("ShaderLoader: ReadAllText vertex failed: {}", vsAbs->string());
            return false;
        }

        auto fsText = ::ddknd::io::ReadAllText(*fsAbs);
        if (!fsText)
        {
            spdlog::error("ShaderLoader: ReadAllText fragment failed: {}", fsAbs->string());
            return false;
        }

        auto prog = backend_.CreateShaderProgram(*vsText, *fsText);
        if (!prog.Is_valid())
        {
            spdlog::error("ShaderLoader: CreateShaderProgram failed");
            return false;
        }

        ::ddknd::graphics::types::ShaderResource res{};
        res.program = prog;

        store.SetLoaded(id, std::move(res));
        assets.SetState(id, ::ddknd::asset::AssetState::Loaded);

        return true;
    }

    //
    bool GraphicsAssetLoader::LoadModel(AssetManager& assets, GraphicsAssetStore& gfxstore,
                                        AnimationAssetStore& animstore, ModelID id)
    {
        auto vpath = assets.TryPathOf<ModelID::tag_type>(id);
        if (!vpath)
        {
            std::cerr << "[LoadModel::TryPathOf] Failed to retrieve vpath.\n";
            return false;
        }
            
        auto path = resolver_.TryResolve(*vpath);
        if (!path)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        const std::string abs = path->string();

        // importer
        auto imported = ddknd::graphics::internal::ImportModel(abs);
        if (!imported)
        {
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

        // const int sceneIndex = imported->defaultScene;
        const int sceneIndex = 0;

        std::vector<AssetID<AnimTag>> clipIdx;

        if (!imported->animations.empty())
        {
            clipIdx = RegisterAnimationClips(assets, *imported, std::string(*vpath));
        }

        // build and set resources
        auto res =
            BuildModelRenderResource(*imported, sceneIndex /*Default SceneIndex*/, backend_, textureMap, std::string(*vpath));

        // The animations within the GLB are treated as sub-assets and given asset IDs
        if (res.nodeToBone)
        {
            for (std::size_t i = 0; i < clipIdx.size(); i++)
            {
                auto anim_res = BuildAnimationClipResource(*imported, i, *res.nodeToBone);
                animstore.SetLoaded(clipIdx[i], std::move(anim_res));
            }
            res.model.clips = std::move(clipIdx);
        }

        gfxstore.SetLoaded(id, std::move(res.model));
        assets.SetState(id, ddknd::asset::AssetState::Loaded);

        return true;
    }

    bool GraphicsAssetLoader::LoadFont(AssetManager& assets, GraphicsAssetStore& store, FontID id)
    {
        constexpr float debugFontSize = 18.0f; // TODO: move to asset data

        if (store.TryGet(id))
            return true;

        auto vpath = assets.TryPathOf(id);
        if (!vpath)
            return false;

        auto path = resolver_.TryResolve(*vpath);
        if (!path)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        const std::string abs = path->string();

        // importer
        auto imported = ::ddknd::graphics::internal::ImportFont(abs, debugFontSize);
        if (!imported)
        {
            assets.SetState(id, ::ddknd::asset::AssetState::Failed);
            return false;
        }

        auto tex = backend_.CreateTextureR8(imported->atlasWidth, imported->atlasHeight, imported->atlasBitmap);

        if (!tex.Is_valid())
        {
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

namespace
{
    template <typename Tag>
    using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

    std::uint32_t ResolveMaterialIndexOrDefault(int importMaterialIndex, std::size_t materialCount)
    {
        if(materialCount == 0)
        {
            return 0;
        }

        if(importMaterialIndex < 0)
        {
            return 0;
        }

        const auto index = static_cast<std::size_t>(importMaterialIndex);

        if(index >= materialCount)
            return 0;

        return static_cast<std::uint32_t>(index);
    }

    ModelBuildResult BuildModelRenderResource(const ImportModelData& import, int sceneIndex, RendererBackned& backend,
                                              std::span<const TextureID> textureMap,const std::string& vpath /*vpath: Model_Asset_Key*/)
    {
        /*
        * @ warn: The glTF specification does not guarantee that nodeIndex > parentNodeIndex.
        * 
        */

        // ====================== build these resource ======================
        // struct PrimitiveResource
        // struct ModelRenderResource
        // =============================== from ===============================
        // struct ModelImportData

        // =============================== alias ============================
        using ModelTag = ::ddknd::graphics::tag::ModelTag;
        using PrimitiveTag = ::ddknd::graphics::tag::PrimitiveTag;

        using PrimitiveResource = ::ddknd::graphics::types::PrimitiveResource;

        // resource container index
        using model_id = GPUID<ModelTag>;
        using prim_id = GPUID<PrimitiveTag>;

        ModelBuildResult out;
        out.nodeToBone.emplace(); // init

        const auto& scenes = import.scenes;
        const auto& nodes = import.nodes;
        const auto& meshes = import.meshes;
        const auto& primitives = import.primitives;

        // ===================== SCENE =====================
        if (sceneIndex < 0 || scenes.size() <= static_cast<std::size_t>(sceneIndex))
        {
            std::cerr << "[BuildModelRenderResource]: Scene Index " << sceneIndex << "does not exist.\n";
            return {};
        }
        // source scene
        out.model.sourceScene = sceneIndex;

        const auto& scene = scenes[sceneIndex];
        // std::cerr << "scene_name=" << scene.name << "\n";

        // ==================== MATERIALS =========================
        out.model.materials.resize(import.materials.size());

        for (std::size_t i = 0; i < import.materials.size(); ++i)
        {
            out.model.materials[i] =
                BuildMaterialResourceFromImport(
                    import,
                    static_cast<std::uint32_t>(i),
                    textureMap
                );
        }

        // fallback
        if(out.model.materials.empty())
        {
            out.model.materials.push_back(MaterialResource{});
        }

        // ===================== NODES =====================
        std::vector<int> roots = scene.rootNodes;
        std::vector<bool> visited(nodes.size(), false);
        std::vector<int> nodeInScene;

        /*
        * @ note:
            The source glTF node array is not guaranteed to be stored in hierarchical order.
            This traversal produces an order where each parent node is appended before its descendants.
            Skeleton bones are built separately from skin.jointNodes,
            whose order is not guaranteed to be parent-before-child.
            If runtime pose evaluation depends on parentBone < childBone,
            BuildModelSkeletonResource must validate or explicitly construct
            a topological bone order.
        */ 
        // to each node
        while (!roots.empty())
        {
            int n = roots.back();
            roots.pop_back();

            if (visited[n])
                continue;

            // std::cerr << "node_name" << nodes[n].name << "\n";
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
                key.prim_index = primIndex;
                // GPU 登録
                auto gpuPrim = backend.CreateOrGetPrimitive(prim, key);
                // std::cerr << "gpu_id = " << gpuPrim.Value() << "\n";
                PrimitiveResource r{};
                r.prim = gpuPrim;
                r.vertexCount = static_cast<std::uint32_t>(prim.vertices.size());
                r.indexCount = static_cast<std::uint32_t>(prim.indices.size());
                r.materialIndex = ResolveMaterialIndexOrDefault(prim.material, out.model.materials.size());

                out.model.primitives.push_back(r);
            }
        }

        for (int n : nodeInScene)
        {
            const auto& skin = nodes[n].skin;
            if (skin >= 0)
            {
                out.model.skeleton = BuildModelSkeletonResource(import, skin, *out.nodeToBone);
                break;
            }
        }

        // print for debugging
        // for(const auto& prim: out.model.primitives)
        // {
        //     std::cerr << "prim_gpu_res_id=" << prim.prim.Value() << "\n";
        // }
        // if(out.model.skeleton)
        // {
        //     for(int i = 0; i < out.model.skeleton->bones.size(); i++)
        //     {
        //         std::cerr << "bone_index=" << i <<", bone_parent_index= " << out.model.skeleton->bones[i].parent << "\n";
        //     }
        // }

        return out;
    }

    int FindBoneIndexFromNode(const ImportSkin& skin, int nodeIndex)
    {
        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            if (skin.jointNodes[i] == nodeIndex)
                return static_cast<int>(i);
        }

        return -1;
    }

    int FindNearestParentBoneIndex(const ImportModelData& import, const ImportSkin& skin, int parentNode,
                                   math::Mat4f& parentCorrection)
    {
        parentCorrection = math::Mat4f::Identity();

        std::vector<int> nonJointParents;

        int n = parentNode;

        while (n >= 0)
        {
            const int boneIndex = FindBoneIndexFromNode(skin, n);

            if (boneIndex >= 0)
            {
                std::reverse(nonJointParents.begin(), nonJointParents.end());

                for (int nodeIndex : nonJointParents)
                {
                    parentCorrection = parentCorrection * import.nodes[nodeIndex].localMatrix;
                }

                return boneIndex;
            }

            nonJointParents.push_back(n);
            n = import.nodes[n].parent;
        }

        std::reverse(nonJointParents.begin(), nonJointParents.end());

        for (int nodeIndex : nonJointParents)
        {
            parentCorrection = parentCorrection * import.nodes[nodeIndex].localMatrix;
        }

        return -1;
    }

    // Convert a node to a bone (runtime).
    // 
    SkeletonResource BuildModelSkeletonResource(const ImportModelData& import, int skinIndex,
                                                std::unordered_map<int, int>& nodeToBone)
    {
        const auto& skin = import.skins[skinIndex];
        SkeletonResource out;
        out.bones.resize(skin.jointNodes.size());

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            // * @note this is not parent-first-order
            const int nodeIndex = skin.jointNodes[i];
            nodeToBone[nodeIndex] = static_cast<int>(i);
        }

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            // * @note this is not parent-first-order
            const int nodeIndex = skin.jointNodes[i];
            const auto& node = import.nodes[nodeIndex];

            Bone b{};

            math::Mat4f parentCorrection = math::Mat4f::Identity();

            b.parent = FindNearestParentBoneIndex(import, skin, node.parent, parentCorrection);

            if (b.parent < 0)
            {
                out.skeletonRootTransform = parentCorrection;
            }
            b.parentCorrection = parentCorrection;
            // std::cerr << "[Bone] i=" << i << " node=" << nodeIndex << " name=" << node.name
            //           << " nodeParent=" << node.parent << " parentBone=" << b.parent << "\n";
            // std::cerr << "parentCorrection:\n"
            //           << b.parentCorrection(0, 0) << " " << b.parentCorrection(0, 1) << " " << b.parentCorrection(0,
            //           2)
            //           << " " << b.parentCorrection(0, 3) << "\n"
            //           << b.parentCorrection(1, 0) << " " << b.parentCorrection(1, 1) << " " << b.parentCorrection(1,
            //           2)
            //           << " " << b.parentCorrection(1, 3) << "\n"
            //           << b.parentCorrection(2, 0) << " " << b.parentCorrection(2, 1) << " " << b.parentCorrection(2,
            //           2)
            //           << " " << b.parentCorrection(2, 3) << "\n"
            //           << b.parentCorrection(3, 0) << " " << b.parentCorrection(3, 1) << " " << b.parentCorrection(3,
            //           2)
            //           << " " << b.parentCorrection(3, 3) << "\n";

            if (i < skin.inverseBindMatrices.size())
                b.inverseBindMatrix = skin.inverseBindMatrices[i];
            else
                b.inverseBindMatrix = math::Mat4f::Identity();

            b.bindLocalTRS = node.localTRS;
            // b.bindLocalMatrix = b.parentCorrection * node.localMatrix;
            b.bindLocalMatrix = node.localMatrix;

            out.bones[i] = b;
        }

        return out;
    }
    /* 
    * @TODO 
        A node is not necessarily a bone (or joint). 
        However, it is necessary to account for the rotation and scaling of nodes that do not correspond to joints.
        This is not currently implemented; a solution is required.
    */
    // parentNode = import.nodes[nodeIndex].parent
    // skin.jointNodes[i]: The index number of the node that has that skin
    // i: 親 node の skin 番号 (親 bone)
    int FindParentBoneIndex(const ImportSkin& skin, int parentNode)
    {
        if (parentNode < 0)
            return -1;

        for (std::size_t i = 0; i < skin.jointNodes.size(); i++)
        {
            if (parentNode == skin.jointNodes[i])
                return static_cast<int>(i);
        }

        return -1;
    }

    // delete
    // std::string MakeAnimationClipKey(std::string_view modelPath, std::size_t animIndex)
    // {
    //     return std::string(modelPath) + "#anim=" + std::to_string(animIndex);
    // }

    // delete
    // std::vector<AssetID<AnimationClipTag>> RegisterAnimationClips(AssetManager& assets, const ImportModelData&
    // imported,
    //                                                               const std::string& vpath /*vpath:
    //                                                               Model_Asset_Key*/)
    // {
    //     std::vector<AssetID<AnimationClipTag>> out;
    //     for (std::size_t i = 0; i < imported.animations.size(); i++)
    //     {
    //         auto key = MakeAnimationClipKey(vpath, i);
    //         auto clipId = assets.GetOrCreate<AnimationClipTag>(key);
    //         out.push_back(clipId);
    //     }

    //     return out;
    // }

    //
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

                std::cerr << "[Skipped animation channel] "
                          << "targetNode=" << ch.targetNode << " name=" << node.name << " parent=" << node.parent
                          << " type=" << static_cast<int>(ch.type) << "\n";

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
            spdlog::warn("[BuildSamplerDescOrDefault]: Invalid samplerindex.");
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
                                                                  graphics::IRendererBackend& backend)
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
        if (!gpuTexture.Is_valid())
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

    std::optional<TextureID> ResolveTextureID(std::span<const TextureID> textureMap, std::optional<std::uint32_t> importTextureIndex)
    {
        if(!importTextureIndex)
            return std::nullopt;

        const auto index = *importTextureIndex;

        if(index >= textureMap.size())
            return std::nullopt;

        const TextureID id = textureMap[index];

        if(!id.Is_valid())
            return std::nullopt;

        return id;
    }

    MaterialResource BuildMaterialResourceFromImport(const ImportModelData& import, std::uint32_t materialIndex,
                                                     std::span<const TextureID> textureMap)
    {
        MaterialResource out{};
        if(materialIndex >= import.materials.size())
            return out;
        const auto& importMaterial = import.materials[materialIndex];
        const auto& pbr = importMaterial.pbrMetallicRoughness;

        // std::cerr << "material name=" << importMaterial.name << "\n";

        // copy factors
        out.baseColorFactor = pbr.baseColorFactor;
        out.metallicFactor = pbr.metallicFactor;
        out.roughnessFactor = pbr.roughnessFactor;
        out.emissiveFactor = importMaterial.emissiveFactor;
        out.alphaMode = importMaterial.alphaMode;
        out.alphaCutoff = importMaterial.alphaCutoff;
        out.doubleSided = importMaterial.doubleSided;

        // resolve ImportIndex - TextureID and copies its datas
        const auto baseColorTexture = ResolveTextureID(textureMap, pbr.baseColorTexture.index);
        if(baseColorTexture)
        {
            out.baseColorTexture.texture = *baseColorTexture;
            out.baseColorTexture.texCoord = pbr.baseColorTexture.texCoord;
        }


        const auto metallicRoughnessTexture = ResolveTextureID(textureMap, pbr.metallicRoughnessTexture.index);
        if(metallicRoughnessTexture)
        {
            out.metallicRoughnessTexture.texture = *metallicRoughnessTexture;
            out.metallicRoughnessTexture.texCoord = pbr.metallicRoughnessTexture.texCoord;
        }
        
        const auto& normal = importMaterial.normalTexture;
        const auto normalTexture = ResolveTextureID(textureMap, normal.index);
        if(normalTexture)
        {
            out.normalTexture.texture = *normalTexture;
            out.normalTexture.texCoord = normal.texCoord;
            out.normalTexture.scale = normal.scale;
        }

        const auto& occlusion = importMaterial.occlusionTexture;
        const auto occlusionTexture = ResolveTextureID(textureMap, occlusion.index);
        if(occlusionTexture)
        {
            out.occlusionTexture.texture = *occlusionTexture;
            out.occlusionTexture.texCoord = occlusion.texCoord;
            out.occlusionTexture.strength = occlusion.strength;
        }

        const auto& emissive = importMaterial.emissiveTexture;
        const auto emissiveTexture = ResolveTextureID(textureMap, emissive.index);
        if(emissiveTexture)
        {
            out.emissiveTexture.texture = *emissiveTexture;
            out.emissiveTexture.texCoord = emissive.texCoord;
        }

        return out;
    }

    // struct TextureResource
    // {
    //     GPUID<tag::TextureGPUTag> gpuTexture;
    //     std::uint32_t width = 0;
    //     std::uint32_t height = 0;
    //     std::uint32_t channels = 0;
    //     graphics::types::TextureFormat format = graphics::types::TextureFormat::RGBA8;
    // };

    // std::vector<math::Mat4f> BuildImportNodeGlobalMatrices(const ImportModelData& import)
    // {
    //     std::vector<math::Mat4f> globals(import.nodes.size(), math::Mat4f::Identity());

    //     std::vector<bool> computed(import.nodes.size(), false);

    //     std::function<void(int)> compute = [&](int nodeIndex)
    //     {
    //         if (computed[nodeIndex])
    //             return;

    //         const auto& node = import.nodes[nodeIndex];

    //         if (node.parent < 0)
    //         {
    //             globals[nodeIndex] = node.localMatrix;
    //         }
    //         else
    //         {
    //             compute(node.parent);
    //             globals[nodeIndex] = globals[node.parent] * node.localMatrix;
    //         }

    //         computed[nodeIndex] = true;
    //     };

    //     for (std::size_t i = 0; i < import.nodes.size(); ++i)
    //     {
    //         compute(static_cast<int>(i));
    //     }

    //     return globals;
    // }

} // namespace

// TODO:
// file asset ではなく sub-asset key を正式導入する。
// 例:
//   model: res://foo.glb#scene=0
//   anim : res://foo.glb#anim=0
//   prim : res://foo.glb#prim=12
//
// AssetManager は key -> AssetID のみ担当し、
// 各 loader/system が key 生成・解釈を担当する。