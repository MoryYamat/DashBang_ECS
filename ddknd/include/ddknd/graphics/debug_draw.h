#pragma once

#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/graphics/type/gfx_resource_types.h>

#include "ddknd/math/math.h"

#include <cstdint>
#include <string>
#include <vector>

namespace ddknd::graphics::debug
{
    struct DebugTextCommand
    {
        float x;
        float y;
        std::string text;
        math::Vec4f color;
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

    class DebugDrawList
    {
      private:
        using Vec3f = math::Vec3f;
        using Vec4f = math::Vec4f;

      public:
        explicit DebugDrawList(IRendererBackend& backend);
        ~DebugDrawList();

        DebugDrawList(const DebugDrawList&) = delete;
        DebugDrawList& operator=(const DebugDrawList&) = delete;
        DebugDrawList(DebugDrawList&&) = delete;
        DebugDrawList& operator=(DebugDrawList&&) = delete;

        void SetFont(const ::ddknd::graphics::types::FontResource* font);

        void BeginFrame();
        void Text(float x, float y, std::string text, Vec4f color);
        void Line(Vec3f a, Vec3f b, Vec4f color);
        void EndFrame();

        // Text
        types::GPUID<tag::ScreenQuadBatchTag> TextBatch() const;
        std::uint32_t TextIndexCount() const;
        types::GPUID<tag::TextureGPUTag> FontAtlas() const;

        // Line
        void Axis(const Vec3f& origin, float length = 1.0f);
        types::GPUID<tag::LineBatchTag> LineBatch() const;
        std::uint32_t LineVertexCount() const;
        void Axis(const Vec3f& origin, const DebugAxisColors& colors, float length = 1.0f);

        void Skeleton(const animation::types::SkeletonResource& skeleton, const animation::types::Pose& pose,
                      Vec4f color);

        void WireSphere(Vec3f center, float radius, Vec4f color, int segments = 24);
        void WireHemisphere(Vec3f center, Vec3f forward, float radius, Vec4f color, int segments = 24);

        void WireCircle(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, Vec4f color, int segments);
        void WireArc(Vec3f center, Vec3f axisA, Vec3f axisB, float radius, float start, float end, Vec4f color,
                     int segments);

      private:
        void BuildTextVertices();
        void FlushText();

        void BuildLineVertices();
        void FlushLine();

      private:
        IRendererBackend& backend_;
        const ::ddknd::graphics::types::FontResource* font_ = nullptr;

        std::vector<DebugTextCommand> texts_;
        types::GPUID<tag::ScreenQuadBatchTag> textBatch_;
        std::vector<types::ScreenQuadVertex> textVertices_;
        std::vector<std::uint32_t> textIndices_;

        std::vector<DebugLineCommand> lines_;
        types::GPUID<tag::LineBatchTag> lineBatch_;
        std::vector<types::LineVertex> lineVertices_;
    };
} // namespace ddknd::graphics::debug