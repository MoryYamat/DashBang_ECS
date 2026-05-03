#include "graphics/renderer.h"

// TODO: need to remove
#include <glad/glad.h>

namespace ddknd::graphics
{
    void RendererSystem::Set_Test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RendererSystem::BeginFrame(FrameDesc& desc)
    {
        // view port
        glViewport(0, 0, desc.w, desc.h);
        //
        glClearColor(0.1f, 0.2f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererSystem::EndFrame()
    {
        for (const auto& cmd : cmds_)
        {
            backend_.UseShaderProgram(cmd.shader);
            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }
    }

    // test triangle
    void RendererSystem::DrawTestTriangle(TestDrawTriangleCommand test)
    {
        backend_.UseShaderProgram(test.shader);
    }
} // namespace ddknd::graphics