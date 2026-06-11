/*
asset のデータを runtime で 保管する 形式 (各種パース済みデータ構造->このデータに変換してasset_storage で 保管)
*/

#pragma once

#include <optional>

#include "ddknd/asset/asset_manager.h"
#include "ddknd/asset/asset_tag.h"
#include "ddknd/math/math.h"

namespace ddknd::graphics::tag
{
    // ========== static ==========
    struct ShaderProgramGPUTag
    {
    };
    struct PrimitiveTag // mesh: vec<primitives>
    {
    };
    struct TextureTag
    {
    };
    struct ScreenQuadBatchTag
    {
    };
    struct LineBatchTag
    {

    };
    // ========== (runtime) =========
    struct ModelTag
    {
    };
} // namespace ddknd::graphics::tag

// delete
namespace ddknd::animation::tag
{
    // delete
    struct AnimationClipTag
    {
    };
} // namespace ddknd::animation::tag

namespace ddknd::graphics::types
{
    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;

    struct PrimitiveKey
    {
        std::string fileKey; // (now:vpath)
        int prim_index = -1; // import.primitives.index

        bool operator==(const PrimitiveKey& o) const
        {
            return prim_index == o.prim_index && fileKey == o.fileKey;
        }
    };

    struct PrimitiveKeyHash
    {
        std::size_t operator()(const PrimitiveKey& k) const
        {
            std::size_t h1 = std::hash<std::string>{}(k.fileKey);
            std::size_t h2 = std::hash<int>{}(k.prim_index);

            return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
        }
    };

    // Font Resource
    struct GlyphResource
    {
      private:
        using Vec2f = math::Vec2f;

      public:
        Vec2f uv0;
        Vec2f uv1;
        Vec2f size;   // glyph bitmap size
        Vec2f offset; // xoff, yoff
        float advance = 0.0f;

        // float u0;
        // float v0;
        // float u1;
        // float v1;

        // float width;
        // float height;

        // float xoff;
        // float yoff;
        // float advance;
    };

    // for drawing fonts
    struct ScreenQuadVertex
    {
        math::Vec2f pos; // pixel座標
        math::Vec2f uv;
        math::Vec4f color;
    };

    // for drawing Line
    struct LineVertex
    {
        math::Vec3f pos;
        math::Vec4f color;
    };
} // namespace ddknd::graphics::types

namespace ddknd::animation::types
{

    struct Bone
    {
        using Mat4f = ::ddknd::math::Mat4f;
        using TRS = ::ddknd::math::TRS;

        int parent = -1;
        Mat4f inverseBindMatrix = Mat4f::Identity();
        Mat4f parentCorrection = Mat4f::Identity();// 
        Mat4f bindLocalMatrix = Mat4f::Identity();
        TRS bindLocalTRS{};
    };

    // runtime
    struct SkeletonResource
    {
        using Mat4f = ::ddknd::math::Mat4f;
        std::vector<Bone> bones;

        Mat4f skeletonRootTransform = Mat4f::Identity();
    };

    enum class ChannelTarget
    {
        Translation,
        Rotation,
        Scale,
    };

    enum class Interpolation
    {
        Linear
    };

    struct AnimationChannel
    {
        using Vec3f = ::ddknd::math::Vec3f;
        using Quatf = ::ddknd::math::Quatf;

        int bone = -1;
        ChannelTarget target;

        std::vector<float> times;
        std::vector<Vec3f> vec3Values;
        std::vector<Quatf> quatValues;

        Interpolation interpolation = Interpolation::Linear;
    };

    struct AnimationClipResource
    {
        std::string name;
        float duration = 0.0f;
        std::vector<AnimationChannel> channels;
    };
} // namespace ddknd::animation::types

// =============================static=============================(it may be internal)
namespace ddknd::graphics::asset
{
    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;

    template <typename Tag>
    using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

    struct ShaderResource
    {
        GPUID<tag::ShaderProgramGPUTag> program;
    };

    struct ModelFileResource
    {
        using ModelTag = ::ddknd::graphics::tag::ModelTag;
        std::vector<AssetID<ModelTag>> scenes;
    };

    struct PrimitiveResource
    {
        GPUID<tag::PrimitiveTag> prim; // GLPrimitive_Index
        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount = 0;
    };

    struct ModelRenderResource // AssetID<tag::Model> model;
    {
        int sourceScene = 0; // default
        std::vector<PrimitiveResource> primitives;
        std::optional<animation::types::SkeletonResource> skeleton;
        // std::vector<graphics::types::AnimationClipResource> clips;
        std::vector<AssetID<::ddknd::asset::tag::AnimationClip>> clips; // Standard clips that can be used with this model
    };

    // FontResource
    struct FontResource
    {
        GPUID<tag::TextureTag> atlas;

        int atlasWidth = 0;
        int atlasHeight = 0;

        int firstCodepoint = 32;
        int glyphCount = 96;

        float pixelHeight = 18.0f;

        std::vector<::ddknd::graphics::types::GlyphResource> glyphs;
    };
} // namespace ddknd::graphics::asset

// =============================runtime=============================
namespace ddknd::animation
{
    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;

    struct Pose
    {
        using TRS = ::ddknd::math::TRS;
        using Mat4f = ::ddknd::math::Mat4f;

        std::vector<TRS> localTRS;
        std::vector<Mat4f> localMatrices;
        std::vector<Mat4f> globalMatrices;
        std::vector<Mat4f> skinMatrices; // lbs?
    };

    struct AnimationState // AssetID<AnimClip> clips
    {
        AssetID<::ddknd::asset::tag::AnimationClip> clip;
        float time = 0.0f;
        float speed = 1.0f;
        bool loop = true;
    };
} // namespace ddknd::animation

namespace ddknd::graphics
{
    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;

    // this is same to ModelComponent
    struct ModelInstance
    {
        AssetID<tag::ModelTag> model; // from ModelStore
        animation::Pose pose;
        animation::AnimationState animState;
    };
} // namespace ddknd::graphics
// ===================================================================

// @TODO: 3Dmodel実行時データ構造の設計とIR->Runtime変換の実装

// GPUID<PrimTag>

// static
// struct ModelResource
// {
//     std::vector<GPU<PrimTag>> meshes;
//     Skeleton skeleton;
//     std::vector<AnimationClip> clips;
// };

// 問題:
// - ModelResource{}自体を保持するStoreを作るか．それとも各リソースを保持するストアにするか
// - ModelResource{}を保持するStoreを作る場合:
//          - 各ResourceとModelResourceの対応関係が必要
//          - 使用側はload(model_id)で簡単
// - ModelResource{}を保持しない場合
//          - 使用側がload(each_id) が必要で大変
//          - ModelResourceの対応関係は不要
// 結論: Resourceを保持するStoreが必要だと思われる
// 懸念点: 例えば、実行時のメッシュやprimitives・pose関係の動的な変化をどう扱うか
// 例 - キャラクターA が Prob_B を 持つ
// というようなものをどうデータの論理で表現するか．ModelResource_Storeを導入するとその管理が大変になるのではないか 予測:
// ModelResource_Storeを導入しなくても、その管理は大変．むしろ論理的には導入したほうが正しい上に扱いやすいと予想される(Instance層で、Model単位の階層関係(もしくは所有関係)を論理的に構築できる)

// 問題2:
// PrimitivesをまとめるMesh層は必要か
//

// runtime
// struct ModelInstance
// {
//     ModelID model;
//     Pose pose;
//     AnimationState animState;
// };

// @TODO
// LOW:
// - Editor 機能: glTF->Scene(Model分割)->sceneIndex を 表示