#pragma once

#include "asset/asset_manager.h"
#include "asset/asset_tag.h"

namespace ddknd::graphics::tag
{
    struct ShaderProgramGPUTag
    {
    };
    struct MeshGPUTag
    {
    };
} // namespace ddknd::graphics::tag

namespace ddknd::graphics::types
{
    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;
} // namespace ddknd::graphics::types

namespace ddknd::graphics::asset
{

    template <typename Tag>
    using ID = ::ddknd::asset::AssetID<Tag>;

    template <typename Tag>
    using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

    struct ShaderResource
    {
        GPUID<tag::ShaderProgramGPUTag> program;
    };

    // GPU ID + meta info
    struct MeshGPUResource
    {
        GPUID<ddknd::graphics::tag::MeshGPUTag> mesh;

        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount = 0;
        bool hasIndices = false;
    };

    struct ModelRenderResource
    {
        std::vector<MeshGPUResource> meshes;
    };
} // namespace ddknd::graphics::asset

namespace ddknd::model::asset
{
    template <typename Tag>
    using ID = ::ddknd::asset::AssetID<Tag>;

    // struct ModelAssetResource
    // {
    //     asset::ID<::ddknd::asset::tag::ModelRender> render;
    //     asset::ID<::ddknd::asset::tag::Skeleton> skeleton;
    //     std::vector<asset::ID<::ddknd::asset::tag::AnimationClip>> clips;
    // };
}// namespace ddknd::model::asset