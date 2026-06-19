#pragma once

// #include "core/StrongID.h"
#include "ddknd/graphics/gfx_type.h"
#include "ddknd/math/math.h"
#include "gfx_type.h"


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

        virtual GPUID<tag::TextureGPUTag> CreateTextureR8(int width, int height, std::span<const std::uint8_t> pixels) = 0;
        virtual void DestroyTexture(GPUID<tag::TextureGPUTag> id) = 0;
        virtual void BindTexture2D(GPUID<tag::TextureGPUTag> id, std::uint32_t slot) = 0;

        //
        virtual GPUID<tag::ScreenQuadBatchTag> CreateScreenQuadBatch() = 0;
        virtual void DestroyScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>) = 0;
        virtual void UpdateScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>, std::span<const types::ScreenQuadVertex>,
                                           std::span<const std::uint32_t>) = 0;
        virtual void DrawScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> batchId, GPUID<tag::ShaderProgramGPUTag> shader,
                                         GPUID<tag::TextureGPUTag> texture, std::uint32_t indexCount, int screenWidth,
                                         int screenHeight) = 0;

        //
        virtual GPUID<tag::LineBatchTag> CreateLineBatch() = 0;
        virtual void UpdateLineBatch(GPUID<tag::LineBatchTag> id, std::span<const types::LineVertex> vertices) = 0;
        virtual void DrawLineBatch(GPUID<tag::LineBatchTag> id, GPUID<tag::ShaderProgramGPUTag> shader,
                                   std::uint32_t vertexCount) = 0;
        virtual void DestroyLineBatch(GPUID<tag::LineBatchTag> id) = 0;

        // Texture
        virtual GPUID<tag::TextureGPUTag> CreateTexture2D(const ::ddknd::graphics::types::Texture2DCreateDesc& desc) = 0;

        // helpers
        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Mat4f& m) = 0;
        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec2f& v) = 0;
        virtual void SetUniformMat4Array(GPUID<tag::ShaderProgramGPUTag> shader, const char* name,
                                         std::span<const math::Mat4f> matrices) = 0;
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

    struct RenderCamera
    {
      ::ddknd::math::Mat4f view;
      ::ddknd::math::Mat4f proj;
      bool valid = false;
    };

    struct SkinnedDrawCommand
    {
      private:
        template <typename Tag>
        using GPUID = ::ddknd::graphics::types::GPUID<Tag>;

      public:
        GPUID<tag::PrimitiveTag> mesh;
        GPUID<tag::ShaderProgramGPUTag> shader;

        math::Mat4f modelMatrix;                   // TransformComponent
        std::span<const math::Mat4f> skinMatrices; // AnimatorComponent

        std::uint32_t indexCount = 0;
    };

    // *********** for Drawing Debug Resources ***********
    struct DebugTextDrawCommand
    {
        types::GPUID<tag::ScreenQuadBatchTag> batch;
        types::GPUID<tag::ShaderProgramGPUTag> shader;
        types::GPUID<tag::TextureGPUTag> texture;

        std::uint32_t indexCount = 0;
    };
    struct DebugLineDrawCommand
    {
        types::GPUID<tag::LineBatchTag> batch;
        types::GPUID<tag::ShaderProgramGPUTag> shader;
        std::uint32_t vertexCount = 0;
    };

    // for testing triangle
    struct TestDrawTriangleCommand
    {
        ddknd::graphics::types::GPUID<tag::PrimitiveTag> mesh;
        ddknd::graphics::types::GPUID<tag::ShaderProgramGPUTag> shader;
        std::uint32_t vcount;
    };

    struct FrameBeginDesc
    {
        int h;
        int w;
        // math::Mat4f view;
        // math::Mat4f proj;
    };

    struct FrameCameraDesc
    {
      ::ddknd::math::Mat4f view;
      ::ddknd::math::Mat4f proj;
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
        void BeginFrame(FrameBeginDesc& des);
        void EndFrame();

        void Submit(const DrawCommand& cmd)
        {
            cmds_.push_back(cmd);
        }
        void Submit(const DebugTextDrawCommand& cmd)
        {
            debugTextCmds_.push_back(cmd);
        }
        void Submit(const DebugLineDrawCommand& cmd)
        {
            debugLineCmds_.push_back(cmd);
        }
        void Submit(const SkinnedDrawCommand& cmd)
        {
            skinnedCmds_.push_back(cmd);
        }

        // test triangle
        void DrawTestTriangle(TestDrawTriangleCommand test);

        void SetFrameCamera(const ::ddknd::graphics::RenderCamera& camera);
      private:
        IRendererBackend& backend_;
        std::vector<DrawCommand> cmds_;
        std::vector<DebugTextDrawCommand> debugTextCmds_;
        std::vector<DebugLineDrawCommand> debugLineCmds_;
        std::vector<SkinnedDrawCommand> skinnedCmds_;
        FrameBeginDesc frameBegin_;
        FrameCameraDesc frameCamera_;
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
        math::Vec3f a;
        math::Vec3f b;
        math::Vec4f color;
    };
    struct DebugAxisColors
    {
        math::Vec4f x{1.0f, 0.0f, 0.0f, 1.0f};
        math::Vec4f y{0.0f, 1.0f, 0.0f, 1.0f};
        math::Vec4f z{0.0f, 0.0f, 1.0f, 1.0f};
    };
    // Create Debug Resources from Commands
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

        void SetFont(const ::ddknd::graphics::types::FontResource* font);

        void BeginFrame();
        void Text(float x, float y, std::string text, Color color);
        void Line(Vec3f a, Vec3f b, Color color);
        void EndFrame();

        // Text
        types::GPUID<tag::ScreenQuadBatchTag> TextBatch() const;
        std::uint32_t TextIndexCount() const;
        types::GPUID<tag::TextureGPUTag> FontAtlas() const;

        // Line
        void Axis(const math::Vec3f& origin, float length = 1.0f);
        types::GPUID<tag::LineBatchTag> LineBatch() const;
        std::uint32_t LineVertexCount() const;
        void Axis(const math::Vec3f& origin, const DebugAxisColors& colors, float length = 1.0f);

        // Skeleton
        void Skeleton(const animation::types::SkeletonResource& skeleton, const animation::types::Pose& pose, Color color);

      private:
        // Text
        void BuildTextVertices();
        void FlushText();

        // Line
        void BuildLineVertices();
        void FlushLine();

      private:
        IRendererBackend& backend_;
        const ::ddknd::graphics::types::FontResource* font_ = nullptr;

        // Text
        std::vector<DebugTextCommand> texts_;
        types::GPUID<tag::ScreenQuadBatchTag> textBatch_;
        std::vector<types::ScreenQuadVertex> textVertices_;
        std::vector<std::uint32_t> textIndices_;

        // Line
        std::vector<DebugLineCommand> lines_;
        types::GPUID<tag::LineBatchTag> lineBatch_;
        std::vector<types::LineVertex> lineVertices_;
        std::vector<std::uint32_t> lineIndices_;
    };

} // namespace ddknd::graphics