#pragma once

#include "core/StrongID.h"
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


// renderer system
namespace ddknd::graphics
{
	// A command that tells the renderer to draw the mesh information for your app
	struct DrawCommand
	{
		ddknd::graphics::types::GPUID<graphics::types::MeshGPUTag> mesh;
		ddknd::graphics::types::GPUID<graphics::types::ShaderProgramGPUTag> shader;
		std::uint32_t vcount;
	};

	// for testing triangle
	struct TestDrawTriangleCommand
	{
		ddknd::graphics::types::GPUID<graphics::types::MeshGPUTag> mesh;
		ddknd::graphics::types::GPUID<graphics::types::ShaderProgramGPUTag> shader;
		std::uint32_t vcount;
	};

	struct FrameDesc
	{
		int h;
		int w;
	};

	class RendererSystem
	{
	public:
		explicit RendererSystem(IRendererBackend& backend) : backend_(backend) {}

		// 
		RendererSystem(const RendererSystem&) = delete;
		RendererSystem& operator=(RendererSystem&) = delete;
		RendererSystem(RendererSystem&&) = delete;
		RendererSystem& operator=(RendererSystem&&) = delete;

		void Set_Test();
		void BeginFrame(FrameDesc& des);
		void EndFrame();

		void Submit(const DrawCommand& cmd)
		{
			cmds_.push_back(cmd);
		}

		// test triangle
		void DrawTestTriangle(TestDrawTriangleCommand test);

	private:
		IRendererBackend& backend_;
		std::vector<DrawCommand> cmds_;
	};

}// namespace ddknd::graphics