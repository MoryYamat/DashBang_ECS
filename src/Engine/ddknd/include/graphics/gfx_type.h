#pragma once

#include "asset/asset_manager.h"

namespace ddknd::graphics::tag
{
    struct ShaderProgramGPUTag{};
    struct MeshGPUTag{};
}// namespace ddknd::graphics::tag

namespace ddknd::graphics::types
{
    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;
} // namespace ddknd::graphics::types


namespace ddknd::graphics::asset
{

    template<typename Tag>
    using ID = ::ddknd::asset::AssetID<Tag>;

    template<typename Tag>
    using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

    struct ShaderResource
    {
        GPUID<tag::ShaderProgramGPUTag> program;
    };

    // GPU ID + meta info
    struct MeshGPUResource
    {
        ddknd::graphics::types::GPUID<
            ddknd::graphics::tag::MeshGPUTag
        > mesh;
        
        std::uint32_t vertexCount = 0;
        std::uint32_t indexCount = 0;
        bool hasIndices = false;
    };

    struct ModelRenderResource
    {
        std::vector<MeshGPUResource> meshes;
    };
}// namespace ddknd::graphics::asset