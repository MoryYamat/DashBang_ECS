#include "graphics/renderer.h"

// TODO: need to remove
#include "math/math.h"
#include <glad/glad.h>

namespace ddknd::graphics
{
    void RendererSystem::Set_Test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RendererSystem::BeginFrame(FrameDesc& desc)
    {
        frame_ = desc;
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

            backend_.SetUniform(cmd.shader, "uView", frame_.view);
            backend_.SetUniform(cmd.shader, "uProj", frame_.proj);
            backend_.SetUniform(cmd.shader, "uModel", math::Mat4f::Identity());

            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }

        for (const auto& cmd : debugTextCmds_)
        {
            backend_.DrawScreenQuadBatch(cmd.batch, cmd.shader, cmd.texture, cmd.indexCount, frame_.w, frame_.h);
        }

        cmds_.clear();
        debugTextCmds_.clear();
    }

    // test triangle
    void RendererSystem::DrawTestTriangle(TestDrawTriangleCommand test)
    {
        backend_.UseShaderProgram(test.shader);
    }

    // =========================== Debug Renderer ===========================
    void DebugDrawList::Init()
    {
        textBatch_ = backend_.CreateScreenQuadBatch();
    }

    void DebugDrawList::SetFont(const asset::FontResource* font)
    {
        if (!font)
        {
            // log warning
            return;
        }
        font_ = font;
    }

    void DebugDrawList::BeginFrame()
    {
        texts_.clear();
    }

    void DebugDrawList::Text(float x, float y, std::string text, Color color)
    {
        texts_.push_back(DebugTextCommand{.x = x, .y = y, .text = std::move(text), .color = color});
    }

    void DebugDrawList::BuildTextVertices()
    {
        textVertices_.clear();
        textIndices_.clear();

        for (const auto& cmd : texts_)
        {
            float penX = cmd.x;
            float penY = cmd.y;

            for (char c : cmd.text)
            {
                const int code = static_cast<unsigned char>(c);
                const int glyphIndex = code - font_->firstCodepoint;

                if (glyphIndex < 0 || glyphIndex >= font_->glyphCount)
                    continue;

                const auto& g = font_->glyphs[glyphIndex];

                const float x0 = penX + g.offset[0];
                const float y0 = penY + g.offset[1];
                const float x1 = x0 + g.size[0];
                const float y1 = y0 + g.size[1];

                const auto base = static_cast<std::uint32_t>(textVertices_.size());

                textVertices_.push_back({{x0, y0}, g.uv0, cmd.color});
                textVertices_.push_back({{x1, y0}, {g.uv1[0], g.uv0[1]}, cmd.color});
                textVertices_.push_back({{x1, y1}, g.uv1, cmd.color});
                textVertices_.push_back({{x0, y1}, {g.uv0[0], g.uv1[1]}, cmd.color});

                textIndices_.push_back(base + 0);
                textIndices_.push_back(base + 1);
                textIndices_.push_back(base + 2);
                textIndices_.push_back(base + 0);
                textIndices_.push_back(base + 2);
                textIndices_.push_back(base + 3);

                penX += g.advance;
            }
        }
    }
    void DebugDrawList::FlushText()
    {
        if (!font_ || texts_.empty())
            return;

        BuildTextVertices();

        backend_.UpdateScreenQuadBatch(textBatch_, textVertices_, textIndices_);
    }

    void DebugDrawList::EndFrame()
    {
        FlushText();
    }
} // namespace ddknd::graphics