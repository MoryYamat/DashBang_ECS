#pragma once

// #include "core/StrongID.h"
#include "gfx_type.h"
#include "math/math.h"

#include <cstdint>
#include <memory>
#include <span>
#include <string_view>

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
        template <typename Tag>
        using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

        using PrimitiveKey = ::ddknd::graphics::types::PrimitiveKey;
        using PrimitiveTag = ::ddknd::graphics::tag::PrimitiveTag;
        using ImportPrimitive = ::ddknd::graphics::internal::types::ImportPrimitive;
        using Mat4f = ::ddknd::math::Mat4f;

      public:
        virtual ~IRendererBackend() = default;
        virtual GPUID<tag::ShaderProgramGPUTag> CreateShaderProgram(std::string_view, std::string_view) = 0;
        virtual void UseShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void DestroyShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void BindPrimitive(GPUID<PrimitiveTag> id) = 0;
        virtual void DrawIndexed(std::uint32_t indexCount) = 0;
        virtual GPUID<PrimitiveTag> CreateMesh_Pos3(std::span<const float> xyz) = 0;
        virtual void DestroyMesh(GPUID<PrimitiveTag> id) = 0;

        virtual GPUID<PrimitiveTag> CreateOrGetPrimitive(const ImportPrimitive&, const PrimitiveKey&) = 0;

        virtual GPUID<tag::TextureTag> CreateTextureR8(int width, int height, std::span<const std::uint8_t> pixels) = 0;
        virtual void DestroyTexture(GPUID<tag::TextureTag> id) = 0;
        virtual void BindTexture2D(GPUID<tag::TextureTag> id, std::uint32_t slot) = 0;

        // virtual void DrawScreenTexturedQuads(std::span<const types::ScreenQuadVertex> vertices,
        //                                      std::span<const std::uint32_t> indices, GPUID<tag::TextureTag> texture,
        //                                      int screenWidth, int screenHeight) = 0;

        //
        virtual GPUID<tag::ScreenQuadBatchTag> CreateScreenQuadBatch() = 0;
        virtual void DestroyScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>) = 0;
        virtual void UpdateScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>, std::span<const types::ScreenQuadVertex>,
                                           std::span<const std::uint32_t>) = 0;
        virtual void DrawScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> batchId, GPUID<tag::ShaderProgramGPUTag> shader,
                                         GPUID<tag::TextureTag> texture, std::uint32_t indexCount, int screenWidth,
                                         int screenHeight) = 0;

        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Mat4f& m) = 0;
        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec2f& v) = 0;
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
        // std::uint32_t vertexCount;
        std::uint32_t indexCount = 0;
    };

    struct DebugTextDrawCommand
    {
        types::GPUID<tag::ScreenQuadBatchTag> batch;
        types::GPUID<tag::ShaderProgramGPUTag> shader;
        types::GPUID<tag::TextureTag> texture;

        std::uint32_t indexCount = 0;
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
        math::Mat4f view;
        math::Mat4f proj;
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
        void Submit(const DebugTextDrawCommand& cmd)
        {
            debugTextCmds_.push_back(cmd);
        }

        // test triangle
        void DrawTestTriangle(TestDrawTriangleCommand test);

      private:
        IRendererBackend& backend_;
        std::vector<DrawCommand> cmds_;
        std::vector<DebugTextDrawCommand> debugTextCmds_;
        FrameDesc frame_;
    };

    // ==================================== DEBUG RENDERER ====================================

    struct DebugTextCommand
    {
      private:
        using Color = math::Vec4f;

      public:
        float x;
        float y;
        std::string text;
        Color color;
    };

    struct DebugLineCommand
    {
    };

    class DebugDrawList
    {
      private:
        using Vec3f = math::Vec3f;
        using Color = math::Vec4f;

      public:
        DebugDrawList(IRendererBackend& backend) : backend_(backend) {}

        DebugDrawList(const RendererSystem&) = delete;
        DebugDrawList& operator=(RendererSystem&) = delete;
        DebugDrawList(RendererSystem&&) = delete;
        DebugDrawList& operator=(RendererSystem&&) = delete;

        void Init();

        void SetFont(const asset::FontResource* font);

        void BeginFrame();
        void Text(float x, float y, std::string text, Color color);
        void Line(Vec3f a, Vec3f b, Color color);
        void EndFrame();

        types::GPUID<tag::ScreenQuadBatchTag> TextBatch() const
        {
            return textBatch_;
        }

        std::uint32_t TextIndexCount() const
        {
            return static_cast<std::uint32_t>(textIndices_.size());
        }

        types::GPUID<tag::TextureTag> FontAtlas() const
        {
          if(!font_)
            return types::GPUID<tag::TextureTag>::Invalid();

          return font_->atlas;
        }

      private:
        void BuildTextVertices();
        void FlushText();

      private:
        IRendererBackend& backend_;
        const asset::FontResource* font_ = nullptr;

        std::vector<DebugTextCommand> texts_;
        std::vector<DebugLineCommand> lines_;

        types::GPUID<tag::ScreenQuadBatchTag> textBatch_;
        std::vector<types::ScreenQuadVertex> textVertices_;
        std::vector<std::uint32_t> textIndices_;
    };

} // namespace ddknd::graphics