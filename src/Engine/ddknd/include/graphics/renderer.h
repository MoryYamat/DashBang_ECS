#pragma once

#include "core/StrongID.hpp"
#include <cstdint>
#include <memory>
#include <string_view>
#include <span>


namespace ddknd::graphics::types
{
    struct ShaderProgramGPUTag
    {
    };
    struct MeshGPUTag
    {
    };

    template <typename GPUTag>
    using GPUID = ddknd::core::StrongID<GPUTag, std::uint32_t>;
} // namespace ddknd::graphics::types

namespace ddknd::graphics
{
    class IRendererBackend
    {
      public:
        virtual ~IRendererBackend() = default;
        virtual types::GPUID<types::ShaderProgramGPUTag> CreateShaderProgram(std::string_view, std::string_view) = 0;
        virtual void UseShaderProgram(types::GPUID<types::ShaderProgramGPUTag> id) = 0;

        virtual void DestroyShaderProgram(types::GPUID<types::ShaderProgramGPUTag> id) = 0;

        virtual void BindMesh(types::GPUID<types::MeshGPUTag> id) = 0;
        virtual void DrawArraysTriangles(std::uint32_t count) = 0;

        virtual types::GPUID<types::MeshGPUTag> CreateMesh_Pos3(std::span<const float> xyz) = 0;
        virtual void DestroyMesh(types::GPUID<types::MeshGPUTag> id) = 0;
    };

    struct OpenGLBackendDesc
    {
    };

    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc);

} // namespace ddknd::graphics