#pragma once

// #include "core/StrongID.h"
#include "gfx_type.h"
#include <cstdint>
#include <memory>
#include <string_view>
#include <span>

// fwd
namespace ddknd::graphics::internal::types
{
	struct ImportPrimitive;
}

namespace ddknd::graphics::types
{
	struct PrimitiveKey;
}


namespace ddknd::graphics
{
    class IRendererBackend
    {
		private:
			template<typename Tag>
			using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

			using PrimitiveKey = ::ddknd::graphics::types::PrimitiveKey;
			using PrimitiveTag = ::ddknd::graphics::tag::PrimitiveTag;
			using ImportPrimitive = ::ddknd::graphics::internal::types::ImportPrimitive;

      public:
        virtual ~IRendererBackend() = default;
        virtual GPUID<tag::ShaderProgramGPUTag> CreateShaderProgram(std::string_view, std::string_view) = 0;
        virtual void UseShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void DestroyShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void BindMesh(GPUID<PrimitiveTag> id) = 0;
        virtual void DrawArraysTriangles(std::uint32_t count) = 0;

        virtual GPUID<PrimitiveTag> CreateMesh_Pos3(std::span<const float> xyz) = 0;
        virtual void DestroyMesh(GPUID<PrimitiveTag> id) = 0;

		virtual GPUID<PrimitiveTag> CreateOrGetPrimitive(const ImportPrimitive&, const PrimitiveKey&) = 0;
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
		ddknd::graphics::types::GPUID<tag::PrimitiveTag> mesh;
		ddknd::graphics::types::GPUID<tag::ShaderProgramGPUTag> shader;
		std::uint32_t vcount;
	};

	// for testing triangle
	struct TestDrawTriangleCommand
	{
		ddknd::graphics::types::GPUID<tag::PrimitiveTag> mesh;
		ddknd::graphics::types::GPUID<tag::ShaderProgramGPUTag> shader;
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