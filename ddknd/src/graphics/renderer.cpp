#include "ddknd/graphics/renderer.h"

#include "internal/graphics/renderer_binding.h"

#include "ddknd/math/math.h"
#include <glad/glad.h>

#include <numbers>

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

    void ResetDefaultRenderState()
    {
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glEnable(GL_CULL_FACE);
    }
}

namespace ddknd::graphics
{
    void RendererSystem::Set_Test()
    {
        glEnable(GL_DEPTH_TEST);
    }

    void RendererSystem::BeginFrame(FrameBeginDesc& desc)
    {
        frameBegin_ = desc;
        // view port
        glViewport(0, 0, desc.w, desc.h);
        //
        glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RendererSystem::EndFrame()
    {
        // Opaque 3D pass
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

            // ********** materials **********
            const bool hasBaseColorTexture = mat.baseColorTexture.texture.Is_valid();

            backend_.SetUniformVec4(cmd.shader, "uBaseColorFactor", mat.baseColorFactor);
            backend_.SetUniformBool(cmd.shader, "uHasBaseColorTexture", hasBaseColorTexture);
            if(hasBaseColorTexture)
            {
                backend_.BindTexture2D(mat.baseColorTexture.texture, graphics::internal::binding::BaseColorTexture);
                backend_.SetUniformInt(cmd.shader, "uBaseColorTexture", graphics::internal::binding::BaseColorTexture);
            }
            // ********************************
            // *********** LIGHTING ***********
            const auto& light = lighting_.mainLight;
            backend_.SetUniformVec3(cmd.shader, "uLightDirWorld", light.directionWolrd);
            backend_.SetUniformVec3(cmd.shader, "uLightColor", light.color * light.intensity);
            backend_.SetUniformFloat(cmd.shader, "uAmbientStrength", lighting_.ambientStrength);
            // ********************************

            backend_.SetUniformMat4Array(cmd.shader, "uSkinMatrices", cmd.skinMatrices);

            backend_.BindPrimitive(cmd.mesh);
            backend_.DrawIndexed(cmd.indexCount);
        }
        
        // overlay
        BeginOverlayPass();

        for (const auto& cmd : debugTextCmds_)
        {
            backend_.DrawScreenQuadBatch(cmd.batch, cmd.shader, cmd.texture, cmd.indexCount, frameBegin_.w,
                                         frameBegin_.h);
        }

        // Debug Line Pass
        BeginLinePass();

        for (const auto& cmd : debugLineCmds_)
        {
            backend_.SetUniform(cmd.shader, "uView", frameCamera_.view);
            backend_.SetUniform(cmd.shader, "uProj", frameCamera_.proj);
            backend_.SetUniform(cmd.shader, "uModel", math::Mat4f::Identity());
            backend_.DrawLineBatch(cmd.batch, cmd.shader, cmd.vertexCount);
        }

        ResetDefaultRenderState();

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

    void RendererSystem::SetFrameCamera(const ::ddknd::graphics::RenderCamera& camera) 
    {
        assert(camera.valid);
        frameCamera_.view = camera.view;
        frameCamera_.proj = camera.proj;
    }

    // =========================== Debug Renderer ===========================
    void DebugDrawList::Init()
    {
        textBatch_ = backend_.CreateScreenQuadBatch();
        lineBatch_ = backend_.CreateLineBatch();
    }

    // @TODO: Dangling pointer countermeasures are needed.
    void DebugDrawList::SetFont(const ::ddknd::graphics::types::FontResource* font)
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

    types::GPUID<tag::TextureGPUTag> DebugDrawList::FontAtlas() const
    {
        if (!font_)
            return types::GPUID<tag::TextureGPUTag>::Invalid();

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

    void DebugDrawList::Axis(const Vec3f& origin, float length)
    {
        const auto x = origin + Vec3f{length, 0.0f, 0.0f};
        const auto y = origin + Vec3f{0.0f, length, 0.0f};
        const auto z = origin + Vec3f{0.0f, 0.0f, length};

        Line(origin, x, {1, 0, 0, 1}); // x red
        Line(origin, y, {0, 1, 0, 1}); // y green
        Line(origin, z, {0, 0, 1, 1}); // z blue
    }

    void DebugDrawList::Axis(const Vec3f& origin, const DebugAxisColors& colors, float length)
    {

        const auto x = origin + Vec3f{length, 0.0f, 0.0f};
        const auto y = origin + Vec3f{0.0f, length, 0.0f};
        const auto z = origin + Vec3f{0.0f, 0.0f, length};

        Line(origin, x, colors.x); // x red
        Line(origin, y, colors.y); // y green
        Line(origin, z, colors.z); // z blue
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
    void DebugDrawList::Skeleton(const animation::types::SkeletonResource& skeleton, const ::ddknd::animation::types::Pose& pose,
                                 Color color)
    {
        for (std::size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            int parent = skeleton.bones[i].parent;
            if (parent < 0)
                continue;
            Vec3f childPos = ExtractTranslation(pose.globalMatrices[i]);
            Vec3f parentPos = ExtractTranslation(pose.globalMatrices[parent]);

            Line(parentPos, childPos, color);
        }
    }

    // WireSphere
    void DebugDrawList::WireSphere(Vec3f center, float radius, Color color, int segments)
    {
        WireCircle(center, Vec3f{1, 0, 0}, Vec3f{0, 1, 0}, radius, color, segments);
        WireCircle(center, Vec3f{0, 1, 0}, Vec3f{0, 0, 1}, radius, color, segments);
        WireCircle(center, Vec3f{0, 0, 1}, Vec3f{1, 0, 0}, radius, color, segments);
    }

    // Hemishpere
    void DebugDrawList::WireHemisphere(Vec3f center, Vec3f forward, float radius, Color color, int segments)
    {
        constexpr float pi = std::numbers::pi_v<float>;
        const Vec3f f = math::normalize(forward);

        const Vec3f tmp = std::abs(f.y()) < 0.99f ? Vec3f{0.0f, 1.0f, 0.0f} : Vec3f{1.0f, 0.0f, 0.0f};

        const Vec3f right = math::normalize(math::cross(tmp, f));
        const Vec3f up = math::normalize(math::cross(f, right));

        // flat cut circle
        WireCircle(center, right, up, radius, color, segments);

        // two forward arcs
        WireArc(center, right, f, radius, 0.0f, pi, color, segments / 2);
        WireArc(center, up, f, radius, 0.0f, pi, color, segments / 2);

        Line(center, center + f * radius, color);
    }
    
    void DebugDrawList::WireCircle(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, Color color, int segments)
    {
        constexpr float pi = std::numbers::pi_v<float>;

        for(int i = 0; i < segments; ++i)
        {
            const float t0 = (2.0f * pi * i) / segments;
            const float t1 = (2.0f * pi * (i + 1)) / segments;

            const auto p0 = center + axisA * (std::cos(t0) * radius) + axisB * (std::sin(t0) * radius);
            const auto p1 = center + axisA * (std::cos(t1) * radius) + axisB * (std::sin(t1) * radius);

            Line(p0, p1, color);
        }
    }

    void DebugDrawList::WireArc(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, float start, float end, Color color, int segments)
    {
        for(int i = 0; i < segments; i++)
        {
            const float t0 = start + (end - start) * (static_cast<float>(i) / segments);
            const float t1 = start + (end - start) * (static_cast<float>(i + 1) / segments);

            const auto p0 = center + axisA * (std::cos(t0) * radius) + axisB * (std::sin(t0) * radius);
            const auto p1 = center + axisA * (std::cos(t1) * radius) + axisB * (std::sin(t1) * radius);

            Line(p0, p1, color);
        }
    }

    void DebugDrawList::EndFrame()
    {
        FlushText();
        FlushLine();
    }
} // namespace ddknd::graphics