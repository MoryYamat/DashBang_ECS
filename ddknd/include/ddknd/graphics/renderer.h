#pragma once

#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/graphics/type/gfx_resource_types.h>
#include <ddknd/graphics/type/render_lighting_types.h>
#include <ddknd/math/math.h>

#include <cstdint>
#include <vector>
#include <span>


// renderer system
namespace ddknd::graphics
{

    struct TextureGPURef
    {
        graphics::types::GPUID<tag::TextureGPUTag> texture;
        std::uint32_t texCoord = 0;
    };

    struct NormalTextureGPURef
    {
        graphics::types::GPUID<tag::TextureGPUTag> texture;
        std::uint32_t texCoord = 0;
        float scale = 1.0f;
    };

    struct OcclusionTextureGPURef
    {
        graphics::types::GPUID<tag::TextureGPUTag> texture;
        std::uint32_t texCoord = 0;
        float strength = 1.0f;
    };

    struct MaterialDrawData
    {
        math::Vec4f baseColorFactor{1, 1, 1, 1};
        TextureGPURef baseColorTexture;

        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;
        TextureGPURef metallicRoughnessTexture;

        NormalTextureGPURef normalTexture;
        OcclusionTextureGPURef occlusionTexture;

        TextureGPURef emissiveTexture;
        math::Vec3f emissiveFactor{0, 0, 0};

        graphics::types::AlphaMode alphaMode = graphics::types::AlphaMode::OPAQUE;
        float alphaCutoff = 0.5f;
        bool doubleSided = false;
    };

    // The reference matrices must remain valid until EndFrame().
    struct DrawCommand
    {
        ddknd::graphics::types::GPUID<tag::PrimitiveTag> mesh;
        ddknd::graphics::types::GPUID<tag::ShaderProgramGPUTag> shader;
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

        math::Mat4f modelMatrix;

        // The referenced matrices must remain valid until EndFrame().
        std::span<const math::Mat4f> skinMatrices;

        std::uint32_t indexCount = 0;

        MaterialDrawData material;
    };

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

    struct FrameBeginDesc
    {
        int w = 0;
        int h = 0;
    };

    struct FrameCameraState
    {
        ::ddknd::math::Mat4f view;
        ::ddknd::math::Mat4f proj;
    };

    class RendererSystem
    {
      public:
        explicit RendererSystem(IRendererBackend& backend) : backend_(backend) {}

        RendererSystem(const RendererSystem&) = delete;
        RendererSystem& operator=(const RendererSystem&) = delete;
        RendererSystem(RendererSystem&&) = delete;
        RendererSystem& operator=(RendererSystem&&) = delete;

        void BeginFrame(FrameBeginDesc& des);
        void EndFrame();

        void Submit(const DrawCommand& cmd);
        void Submit(const DebugTextDrawCommand& cmd);
        void Submit(const DebugLineDrawCommand& cmd);
        void Submit(const SkinnedDrawCommand& cmd);

        void SetFrameCamera(const RenderCamera& camera);

      private:
        IRendererBackend& backend_;
        std::vector<DrawCommand> cmds_;
        std::vector<DebugTextDrawCommand> debugTextCmds_;
        std::vector<DebugLineDrawCommand> debugLineCmds_;
        std::vector<SkinnedDrawCommand> skinnedCmds_;
        FrameBeginDesc frameBegin_;
        FrameCameraState frameCamera_;

        // simple light
        graphics::types::RenderLighting lighting_{};
    };

    

} // namespace ddknd::graphics