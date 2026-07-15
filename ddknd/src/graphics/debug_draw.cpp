#include "ddknd/graphics/debug_draw.h"

#include "ddknd/graphics/renderer_backend.h"

#include <cassert>
#include <utility>
#include <cmath>
#include <numbers>

namespace ddknd::graphics::debug
{

    DebugDrawList::DebugDrawList(IRendererBackend& backend) : backend_(backend) 
    {
        textBatch_ = backend_.CreateScreenQuadBatch();
        lineBatch_ = backend_.CreateLineBatch();
    }

    DebugDrawList::~DebugDrawList()
    {
        if(textBatch_.IsValid())
        {
            backend_.DestroyScreenQuadBatch(textBatch_);
        }

        if(lineBatch_.IsValid())
        {
            backend_.DestroyLineBatch(lineBatch_);
        }
    }

    // FontResource must outlive this DebugDrawList.
    void DebugDrawList::SetFont(const ::ddknd::graphics::types::FontResource* font)
    {
        if (!font)
        {
            return;
        }
        font_ = font;
    }

    void DebugDrawList::BeginFrame()
    {
        texts_.clear();
        lines_.clear();

        textVertices_.clear();
        textIndices_.clear();

        lineVertices_.clear();
    }

    void DebugDrawList::Text(float x, float y, std::string text, Vec4f color)
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

    void DebugDrawList::Line(Vec3f a, Vec3f b, Vec4f color)
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

        Line(origin, x, {1, 0, 0, 1});
        Line(origin, y, {0, 1, 0, 1});
        Line(origin, z, {0, 0, 1, 1});
    }

    void DebugDrawList::Axis(const Vec3f& origin, const DebugAxisColors& colors, float length)
    {

        const auto x = origin + Vec3f{length, 0.0f, 0.0f};
        const auto y = origin + Vec3f{0.0f, length, 0.0f};
        const auto z = origin + Vec3f{0.0f, 0.0f, length};

        Line(origin, x, colors.x);
        Line(origin, y, colors.y);
        Line(origin, z, colors.z);
    }

    types::GPUID<tag::LineBatchTag> DebugDrawList::LineBatch() const
    {
        return lineBatch_;
    }
    std::uint32_t DebugDrawList::LineVertexCount() const
    {
        return static_cast<std::uint32_t>(lineVertices_.size());
    }

    void DebugDrawList::Skeleton(const animation::types::SkeletonResource& skeleton,
                                 const ::ddknd::animation::types::Pose& pose, Vec4f color)
    {
        assert(pose.globalMatrices.size() >= skeleton.bones.size());
        for (std::size_t i = 0; i < skeleton.bones.size(); ++i)
        {
            int parent = skeleton.bones[i].parent;
            if (parent < 0)
                continue;

            assert(static_cast<std::size_t>(parent) < pose.globalMatrices.size());
            
            Vec3f childPos = ExtractTranslation(pose.globalMatrices[i]);
            Vec3f parentPos = ExtractTranslation(pose.globalMatrices[parent]);

            Line(parentPos, childPos, color);
        }
    }

    void DebugDrawList::WireSphere(Vec3f center, float radius, Vec4f color, int segments)
    {
        WireCircle(center, Vec3f{1, 0, 0}, Vec3f{0, 1, 0}, radius, color, segments);
        WireCircle(center, Vec3f{0, 1, 0}, Vec3f{0, 0, 1}, radius, color, segments);
        WireCircle(center, Vec3f{0, 0, 1}, Vec3f{1, 0, 0}, radius, color, segments);
    }

    void DebugDrawList::WireHemisphere(Vec3f center, Vec3f forward, float radius, Vec4f color, int segments)
    {
        constexpr float pi = std::numbers::pi_v<float>;
        const Vec3f f = math::normalize(forward);

        const Vec3f tmp = std::abs(f.y()) < 0.99f ? Vec3f{0.0f, 1.0f, 0.0f} : Vec3f{1.0f, 0.0f, 0.0f};

        const Vec3f right = math::normalize(math::cross(tmp, f));
        const Vec3f up = math::normalize(math::cross(f, right));

        // Draw the flat boundary circle.
        WireCircle(center, right, up, radius, color, segments);

        // Draw two forward-facing arcs.
        WireArc(center, right, f, radius, 0.0f, pi, color, segments / 2);
        WireArc(center, up, f, radius, 0.0f, pi, color, segments / 2);

        Line(center, center + f * radius, color);
    }

    void DebugDrawList::WireCircle(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, Vec4f color, int segments)
    {
        constexpr float pi = std::numbers::pi_v<float>;

        for (int i = 0; i < segments; ++i)
        {
            const float t0 = (2.0f * pi * i) / segments;
            const float t1 = (2.0f * pi * (i + 1)) / segments;

            const auto p0 = center + axisA * (std::cos(t0) * radius) + axisB * (std::sin(t0) * radius);
            const auto p1 = center + axisA * (std::cos(t1) * radius) + axisB * (std::sin(t1) * radius);

            Line(p0, p1, color);
        }
    }

    void DebugDrawList::WireArc(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, float start, float end,
                                Vec4f color, int segments)
    {
        for (int i = 0; i < segments; i++)
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
} // namespace ddknd::graphics::debug