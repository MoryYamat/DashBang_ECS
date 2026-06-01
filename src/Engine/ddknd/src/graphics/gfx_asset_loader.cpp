#include "graphics/gfx_asset_loader.h"

#include <cstddef>
#include <cstdint>

#include "asset/asset_manager.h"

#include "graphics/model_data.h"
#include "graphics/renderer.h"
#include "internal/asset/shader_descriptor_parser.h"
#include "internal/graphics/model_importer/model_import_types.h"
#include "internal/io/io.h"

#include "internal/graphics/model_importer/glb_importer.h"

#include "internal/graphics/font_importer/stb_font_importer.h"

#include "graphics/gfx_type.h"
#include "math/math.h"

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
    using AnimationClipTag = ::ddknd::animation::tag::AnimationClipTag;
    using ImportChannelType = ddknd::graphics::internal::types::ChannelType;

    using ModelRenderResource = ::ddknd::graphics::asset::ModelRenderResource;
    using SkeletonResource = ::ddknd::animation::types::SkeletonResource;
    using AnimationChannel = ::ddknd::animation::types::AnimationChannel;
    using Bone = ::ddknd::animation::types::Bone;

    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;
    using AssetManager = ::ddknd::asset::AssetManager;

    using GraphicsAssetStore = ::ddknd::graphics::GraphicsAssetStore;
    using RendererBackned = ::ddknd::graphics::IRendererBackend;
    using PrimitiveKey = ::ddknd::graphics::types::PrimitiveKey;

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
                                              const std::string& vpath /*vpath: Model_Asset_Key*/);
    SkeletonResource BuildModelSkeletonResource(const ImportModelData& import, int skinIndex,
                                                std::unordered_map<int, int>&);
    int FindParentBoneIndex(const ImportSkin& skin, int parentNode);

    // std::vector<math::Mat4f> BuildImportNodeGlobalMatrices(const ImportModelData& import);

    // Animation
    AnimationClipResource BuildAnimationClipResource(const ImportModelData&, const std::size_t,
                                                     const std::unordered_map<int, int>&);

    // temporaly
    std::vector<AssetID<AnimationClipTag>> RegisterAnimationClips(AssetManager&, const ImportModelData&,
                                                                  const std::string& vpath /*vpath: Model_Asset_Key*/);
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

        asset::ShaderResource res{};
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
            return false;

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

        // const int sceneIndex = imported->defaultScene;
        const int sceneIndex = 0;

        std::vector<AssetID<AnimationClipTag>> clipIdx;

        if (!imported->animations.empty())
        {
            clipIdx = RegisterAnimationClips(assets, *imported, std::string(*vpath));
        }

        // build and set resources
        auto res =
            BuildModelRenderResource(*imported, sceneIndex /*Default SceneIndex*/, backend_, std::string(*vpath));

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
        asset::FontResource fontRes;

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

    ModelBuildResult BuildModelRenderResource(const ImportModelData& import, int sceneIndex, RendererBackned& backend,
                                              const std::string& vpath)
    {
        // ====================== build these resource ======================
        // struct PrimitiveResource
        // struct ModelRenderResource
        // =============================== from ===============================
        // struct ModelImportData

        // =============================== alias ============================
        using ModelTag = ::ddknd::graphics::tag::ModelTag;
        using PrimitiveTag = ::ddknd::graphics::tag::PrimitiveTag;

        using PrimitiveResource = ::ddknd::graphics::asset::PrimitiveResource;

        // resource container index
        using model_id = GPUID<ModelTag>;
        using prim_id = GPUID<PrimitiveTag>;

        ModelBuildResult result;
        result.nodeToBone.emplace(); // init

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
        result.model.sourceScene = sceneIndex;

        const auto& scene = scenes[sceneIndex];
        // std::cerr << "scene_name=" << scene.name << "\n";

        // ===================== NODES =====================
        std::vector<int> roots = scene.rootNodes;
        std::vector<bool> visited(nodes.size(), false);
        std::vector<int> nodeInScene;

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

                result.model.primitives.push_back(r);
            }
        }

        for (int n : nodeInScene)
        {
            const auto& skin = nodes[n].skin;
            if (skin >= 0)
            {
                result.model.skeleton = BuildModelSkeletonResource(import, skin, *result.nodeToBone);
                break;
            }
        }

        // for(const auto& prim: out.primitives)
        // {
        //     std::cerr << "prim_gpu_res_id=" << prim.prim.Value() << "\n";
        // }
        // if(out.skeleton)
        // {
        //     for(int i = 0; i < out.skeleton->bones.size(); i++)
        //     {
        //         std::cerr << "bone_parent_index= " << out.skeleton->bones[i].parent << "\n";
        //     }
        // }

        return result;
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

    SkeletonResource BuildModelSkeletonResource(const ImportModelData& import, int skinIndex,
                                                std::unordered_map<int, int>& nodeToBone)
    {
        const auto& skin = import.skins[skinIndex];

        SkeletonResource out;
        out.bones.resize(skin.jointNodes.size());

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            const int nodeIndex = skin.jointNodes[i];
            nodeToBone[nodeIndex] = static_cast<int>(i);
        }

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            const int nodeIndex = skin.jointNodes[i];
            const auto& node = import.nodes[nodeIndex];

            Bone b{};

            math::Mat4f parentCorrection = math::Mat4f::Identity();

            b.parent = FindNearestParentBoneIndex(import, skin, node.parent, parentCorrection);
            
            if (b.parent < 0)
            {
                out.rootCorrection = ExtractRotationOnly(parentCorrection);
            }
            b.parentCorrection = parentCorrection;
            std::cerr << "[Bone] i=" << i << " node=" << nodeIndex << " name=" << node.name
                      << " nodeParent=" << node.parent << " parentBone=" << b.parent << "\n";
            std::cerr << "parentCorrection:\n"
                      << b.parentCorrection(0, 0) << " " << b.parentCorrection(0, 1) << " " << b.parentCorrection(0, 2)
                      << " " << b.parentCorrection(0, 3) << "\n"
                      << b.parentCorrection(1, 0) << " " << b.parentCorrection(1, 1) << " " << b.parentCorrection(1, 2)
                      << " " << b.parentCorrection(1, 3) << "\n"
                      << b.parentCorrection(2, 0) << " " << b.parentCorrection(2, 1) << " " << b.parentCorrection(2, 2)
                      << " " << b.parentCorrection(2, 3) << "\n"
                      << b.parentCorrection(3, 0) << " " << b.parentCorrection(3, 1) << " " << b.parentCorrection(3, 2)
                      << " " << b.parentCorrection(3, 3) << "\n";

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

    // tmp
    std::string MakeAnimationClipKey(std::string_view modelPath, std::size_t animIndex)
    {
        return std::string(modelPath) + "#anim=" + std::to_string(animIndex);
    }

    std::vector<AssetID<AnimationClipTag>> RegisterAnimationClips(AssetManager& assets, const ImportModelData& imported,
                                                                  const std::string& vpath /*vpath: Model_Asset_Key*/)
    {
        std::vector<AssetID<AnimationClipTag>> out;
        for (std::size_t i = 0; i < imported.animations.size(); i++)
        {
            auto key = MakeAnimationClipKey(vpath, i);
            auto clipId = assets.GetOrCreate<AnimationClipTag>(key);
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