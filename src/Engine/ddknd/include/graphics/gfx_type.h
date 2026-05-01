#pragma once

#include <optional>

#include "asset/asset_manager.h"
#include "asset/asset_tag.h"
#include "math/math.h"

namespace ddknd::graphics::tag
{
    // ========== static ==========
    struct ShaderProgramGPUTag
    {
    };
    struct PrimitiveTag // mesh: vec<primitives>
    {
    };
    // ========== runtime =========
    struct ModelTag
    {
    };
} // namespace ddknd::graphics::tag

namespace ddknd::animation::tag
{
    struct AnimationClipTag
    {
    };
}// namespace ddknd::animation::tag

namespace ddknd::graphics::types
{
    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;

    struct Bone
    {
        using Mat4f = ::ddknd::math::Mat4f;
        using TRS = ::ddknd::math::TRS;

        int parent = -1;
        Mat4f inverseBindMatrix = Mat4f::Identity();
        TRS bindLocal;
    };

    struct SkeletonResource
    {
        std::vector<Bone> bones;

        std::vector<int> boneToNode;
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
} // namespace ddknd::graphics::types

// =============================static=============================
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

    struct PrimitiveResource
    {
        GPUID<tag::PrimitiveTag> prim;// GLPrimitive_Index
        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount = 0;
    };

    struct ModelRenderResource      // AssetID<tag::Model> model;
    {
        std::vector<PrimitiveResource> primitives;
        std::optional<graphics::types::SkeletonResource> skeleton;
        // std::vector<graphics::types::AnimationClipResource> clips;
        std::vector<AssetID<animation::tag::AnimationClipTag>> clips;// Standard clips that can be used with this model
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

        std::vector<TRS> localTransforms;
        std::vector<Mat4f> globalMatrices;
        std::vector<Mat4f> skinMatrices; // lbs?
    };

    struct AnimationState// AssetID<AnimClip> clips
    {
        AssetID<tag::AnimationClipTag> clip;
        float time = 0.0f;
        bool loop = false;
    };
} // namespace ddknd::animation

namespace ddknd::graphics
{
    template <typename Tag>
    using AssetID = ::ddknd::asset::AssetID<Tag>;

    // this is same to ModelComponent
    struct ModelInstance
    {
        AssetID<tag::ModelTag> model;// from ModelStore
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