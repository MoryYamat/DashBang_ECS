#pragma once

#include <string_view>
#include <span>
#include <cstdint>
#include <memory>

#include <ddknd/graphics/type/gpu_types.h>



// Interface
namespace ddknd::graphics
{
    class IRendererBackend
    {
      private:
        template <typename Tag>
        using GPUID = ddknd::graphics::types::GPUID<Tag>;

        using PrimitiveKey = ddknd::graphics::types::PrimitiveKey;
        using PrimitiveTag = ddknd::graphics::tag::PrimitiveTag;
        using PrimitiveCreateData = ddknd::graphics::types::PrimitiveCreateData;
        using Mat4f = ddknd::math::Mat4f;

      public:
        virtual ~IRendererBackend() = default;
        virtual GPUID<tag::ShaderProgramGPUTag> CreateShaderProgram(std::string_view, std::string_view) = 0;
        virtual void UseShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void DestroyShaderProgram(GPUID<tag::ShaderProgramGPUTag> id) = 0;

        virtual void BindPrimitive(GPUID<PrimitiveTag> id) = 0;
        virtual void DrawIndexed(std::uint32_t indexCount) = 0;
        virtual GPUID<PrimitiveTag> CreateMesh_Pos3(std::span<const float> xyz) = 0;
        virtual void DestroyMesh(GPUID<PrimitiveTag> id) = 0;

        virtual GPUID<PrimitiveTag> CreateOrGetPrimitive(const PrimitiveCreateData&, const PrimitiveKey&) = 0;

        virtual GPUID<tag::TextureGPUTag> CreateTextureR8(int width, int height,
                                                          std::span<const std::uint8_t> pixels) = 0;
        virtual void DestroyTexture(GPUID<tag::TextureGPUTag> id) = 0;
        virtual void BindTexture2D(GPUID<tag::TextureGPUTag> id, std::uint32_t slot) = 0;

        virtual GPUID<tag::ScreenQuadBatchTag> CreateScreenQuadBatch() = 0;
        virtual GPUID<tag::ScreenQuadBatchTag> CreateScreenQuadBatchWithoutTexture() = 0;
        virtual void DestroyScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>) = 0;
        virtual void UpdateScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag>, std::span<const types::ScreenQuadVertex>,
                                           std::span<const std::uint32_t>) = 0;
        virtual void DrawScreenQuadBatch(GPUID<tag::ScreenQuadBatchTag> batchId, GPUID<tag::ShaderProgramGPUTag> shader,
                                         GPUID<tag::TextureGPUTag> texture, std::uint32_t indexCount, int screenWidth,
                                         int screenHeight) = 0;

        virtual void DrawScreenQuadBatchWithoutTexture(GPUID<tag::ScreenQuadBatchTag> batchId, GPUID<tag::ShaderProgramGPUTag> shader,
                                         std::uint32_t indexCount, int screenWidth, int screenHeight) = 0;


        virtual GPUID<tag::LineBatchTag> CreateLineBatch() = 0;
        virtual void UpdateLineBatch(GPUID<tag::LineBatchTag> id, std::span<const types::LineVertex> vertices) = 0;
        virtual void DrawLineBatch(GPUID<tag::LineBatchTag> id, GPUID<tag::ShaderProgramGPUTag> shader,
                                   std::uint32_t vertexCount) = 0;
        virtual void DestroyLineBatch(GPUID<tag::LineBatchTag> id) = 0;

        virtual GPUID<tag::TextureGPUTag> CreateTexture2D(
            const ::ddknd::graphics::types::Texture2DCreateDesc& desc) = 0;

        
        virtual void SetUniformInt(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const int v) = 0;
        virtual void SetUniformBool(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const int v) = 0;
        virtual void SetUniformFloat(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const float v) = 0;
        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Mat4f& m) = 0;
        virtual void SetUniform(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec2f& v) = 0;
        virtual void SetUniformVec3(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec3f& v) = 0;
        virtual void SetUniformVec4(GPUID<tag::ShaderProgramGPUTag> shader, const char* name, const math::Vec4f& v) = 0;
        virtual void SetUniformMat4Array(GPUID<tag::ShaderProgramGPUTag> shader, const char* name,
                                         std::span<const math::Mat4f> matrices) = 0;
    };

    struct OpenGLBackendDesc
    {
    };

    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc);

} // namespace ddknd::graphics
