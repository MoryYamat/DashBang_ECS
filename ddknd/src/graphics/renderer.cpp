#include "ddknd/graphics/renderer.h"

// TODO: need to remove
#include "ddknd/math/math.h"
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
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
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
        for(const auto& cmd: skinnedCmds_)
        {
            backend_.UseShaderProgram(cmd.shader);

            backend_.SetUniform(cmd.shader, "uModel", cmd.modelMatrix);
            backend_.SetUniform(cmd.shader, "uView", frame_.view);
            backend_.SetUniform(cmd.shader, "uProj", frame_.proj);

            backend_.SetUniformMat4Array(
                cmd.shader,
                "uSkinMatrices",
                cmd.skinMatrices
            );
            
            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }

        for (const auto& cmd : debugTextCmds_)
        {
            backend_.DrawScreenQuadBatch(cmd.batch, cmd.shader, cmd.texture, cmd.indexCount, frame_.w, frame_.h);
        }
        for (const auto& cmd : debugLineCmds_)
        {
            backend_.SetUniform(cmd.shader, "uView", frame_.view);
            backend_.SetUniform(cmd.shader, "uProj", frame_.proj);
            backend_.SetUniform(cmd.shader, "uModel", math::Mat4f::Identity());
            backend_.DrawLineBatch(cmd.batch, cmd.shader, cmd.vertexCount);
        }

        cmds_.clear();
        skinnedCmds_.clear();
        debugTextCmds_.clear();
        debugLineCmds_.clear();
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
        lineBatch_ = backend_.CreateLineBatch();
    }

    // @TODO: Dangling pointer countermeasures are needed.
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
        lines_.clear();
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

    types::GPUID<tag::ScreenQuadBatchTag> DebugDrawList::TextBatch() const
    {
        return textBatch_;
    }

    std::uint32_t DebugDrawList::TextIndexCount() const
    {
        return static_cast<std::uint32_t>(textIndices_.size());
    }

    types::GPUID<tag::TextureTag> DebugDrawList::FontAtlas() const
    {
        if (!font_)
            return types::GPUID<tag::TextureTag>::Invalid();

        return font_->atlas;
    }

    // ************ LINE *************
    void DebugDrawList::Line(Vec3f a, Vec3f b, Color color)
    {
        lines_.push_back(DebugLineCommand{.a = a, .b = b, .color = color});
    }
    void DebugDrawList::BuildLineVertices()
    {
        lineVertices_.clear();
        for (const auto& cmd : lines_)
        {
            lineVertices_.push_back({cmd.a, cmd.color});
            lineVertices_.push_back({cmd.b, cmd.color});
        }
    }
    void DebugDrawList::FlushLine()
    {
        if (lines_.empty())
            return;
        BuildLineVertices();
        backend_.UpdateLineBatch(lineBatch_, lineVertices_);
    }

    void DebugDrawList::Axis(const math::Vec3f& origin, float length)
    {
        const auto x = origin + math::Vec3f{length, 0.0f, 0.0f};
        const auto y = origin + math::Vec3f{0.0f, length, 0.0f};
        const auto z = origin + math::Vec3f{0.0f, 0.0f, length};

        Line(origin, x, {1,0,0,1});// x red
        Line(origin, y, {0,1,0,1});// y green
        Line(origin, z, {0,0,1,1});// z blue
    }

    void DebugDrawList::Axis(const math::Vec3f& origin, const DebugAxisColors& colors, float length)
    {

        const auto x = origin + math::Vec3f{length, 0.0f, 0.0f};
        const auto y = origin + math::Vec3f{0.0f, length, 0.0f};
        const auto z = origin + math::Vec3f{0.0f, 0.0f, length};

        Line(origin, x, colors.x);// x red
        Line(origin, y, colors.y);// y green
        Line(origin, z, colors.z);// z blue
    }
    
    types::GPUID<tag::LineBatchTag> DebugDrawList::LineBatch() const
    {
        return lineBatch_;
    }
    std::uint32_t DebugDrawList::LineVertexCount() const
    {
        return static_cast<std::uint32_t>(lineVertices_.size());
    }

    // *************** SKELETON ***************** 
    void DebugDrawList::Skeleton(const animation::types::SkeletonResource& skeleton, const animation::Pose& pose, Color color)
    {
        for(std::size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            int parent = skeleton.bones[i].parent;
            if(parent < 0)
                continue;
            Vec3f childPos = ExtractTranslation(pose.globalMatrices[i]);
            Vec3f parentPos = ExtractTranslation(pose.globalMatrices[parent]);

            Line(parentPos, childPos, color);
        }
    }

    void DebugDrawList::EndFrame()
    {
        FlushText();
        FlushLine();
    }
} // namespace ddknd::graphics