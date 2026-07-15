#include "ddknd/graphics/renderer.h"

#include "ddknd/graphics/renderer_backend.h"

#include "internal/graphics/renderer_binding.h"


#include "ddknd/math/math.h"
#include <glad/glad.h>

#include <cassert>

namespace 
{
    void BeginOpaquePass()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_TRUE);

        glDisable(GL_BLEND);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    void BeginLinePass()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glDepthMask(GL_FALSE);

        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

    void BeginOverlayPass()
    {
        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);

        glDisable(GL_CULL_FACE);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glDisable(GL_SCISSOR_TEST);
        glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }

}

namespace ddknd::graphics
{
    void RendererSystem::BeginFrame(FrameBeginDesc& desc)
    {
        frameBegin_ = desc;

        // Restore the render state for frame clear and opaque rendering.
        BeginOpaquePass();

        glViewport(0, 0, desc.w, desc.h);
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererSystem::EndFrame()
    {
        BeginOpaquePass();

        for (const auto& cmd : cmds_)
        {
            backend_.UseShaderProgram(cmd.shader);

            backend_.SetUniform(cmd.shader, "uView", frameCamera_.view);
            backend_.SetUniform(cmd.shader, "uProj", frameCamera_.proj);
            backend_.SetUniform(cmd.shader, "uModel", math::Mat4f::Identity());

            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }
        
        for (const auto& cmd : skinnedCmds_)
        {
            const auto& mat = cmd.material;
            backend_.UseShaderProgram(cmd.shader);

            backend_.SetUniform(cmd.shader, "uModel", cmd.modelMatrix);
            backend_.SetUniform(cmd.shader, "uView", frameCamera_.view);
            backend_.SetUniform(cmd.shader, "uProj", frameCamera_.proj);

            // Material parameters
            const bool hasBaseColorTexture = mat.baseColorTexture.texture.IsValid();

            backend_.SetUniformVec4(cmd.shader, "uBaseColorFactor", mat.baseColorFactor);
            backend_.SetUniformBool(cmd.shader, "uHasBaseColorTexture", hasBaseColorTexture);
            
            if(hasBaseColorTexture)
            {
                backend_.BindTexture2D(mat.baseColorTexture.texture, graphics::internal::binding::BaseColorTexture);
                backend_.SetUniformInt(cmd.shader, "uBaseColorTexture", graphics::internal::binding::BaseColorTexture);
            }

            // Main directional light
            const auto& light = lighting_.mainLight;
            backend_.SetUniformVec3(cmd.shader, "uLightDirWorld", light.directionWolrd);
            backend_.SetUniformVec3(cmd.shader, "uLightColor", light.color * light.intensity);
            backend_.SetUniformFloat(cmd.shader, "uAmbientStrength", lighting_.ambientStrength);

            backend_.SetUniformMat4Array(cmd.shader, "uSkinMatrices", cmd.skinMatrices);

            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }
        
        BeginOverlayPass();

        for (const auto& cmd : debugTextCmds_)
        {
            backend_.DrawScreenQuadBatch(cmd.batch, cmd.shader, cmd.texture, cmd.indexCount, frameBegin_.w,
                                         frameBegin_.h);
        }

        BeginLinePass();

        for (const auto& cmd : debugLineCmds_)
        {
            backend_.SetUniform(cmd.shader, "uView", frameCamera_.view);
            backend_.SetUniform(cmd.shader, "uProj", frameCamera_.proj);
            backend_.SetUniform(cmd.shader, "uModel", math::Mat4f::Identity());
            backend_.DrawLineBatch(cmd.batch, cmd.shader, cmd.vertexCount);
        }

        cmds_.clear();
        skinnedCmds_.clear();
        debugTextCmds_.clear();
        debugLineCmds_.clear();
    }

    void RendererSystem::Submit(const DrawCommand& cmd)
    {
        cmds_.push_back(cmd);
    }
    void RendererSystem::Submit(const DebugTextDrawCommand& cmd)
    {
        debugTextCmds_.push_back(cmd);
    }
    void RendererSystem::Submit(const DebugLineDrawCommand& cmd)
    {
        debugLineCmds_.push_back(cmd);
    }
    void RendererSystem::Submit(const SkinnedDrawCommand& cmd)
    {
        skinnedCmds_.push_back(cmd);
    }

    void RendererSystem::SetFrameCamera(const ::ddknd::graphics::RenderCamera& camera) 
    {
        assert(camera.valid);
        frameCamera_.view = camera.view;
        frameCamera_.proj = camera.proj;
    }
} // namespace ddknd::graphics