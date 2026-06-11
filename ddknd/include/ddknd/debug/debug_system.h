#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/system/system_fwd.h>

#include <ddknd/asset/asset_manager.h>
#include <ddknd/asset/asset_tag.h>
#include <ddknd/math/math.h>

namespace ddknd::debug
{
    struct DebugFPSTextStyle
    {
    private:
        using Color = ::ddknd::math::Vec4f;
      public:
        Color color{1.0f, 1.0f, 0.0f, 1.0f};
    };

    struct DebugSkeletonStyle
    {
        ::ddknd::math::Vec4f color{0.0f, 1.0f, 1.0f, 1.0f};
    };

    struct DebugAxisStyle
    {
      private:
        using Color = ::ddknd::math::Vec4f;

      public:
        Color axisX{1.0f, 0.0f, 0.0f, 1.0f};
        Color axisY{0.0f, 1.0f, 0.0f, 1.0f};
        Color axisZ{0.0f, 0.0f, 1.0f, 1.0f};

        ::ddknd::math::Vec3f origin{0.0f, 0.0f, 0.0f};
        float length = 5.0f;
    };

    struct DebugDrawResources
    {
        // shaders
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> textShader;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Shader> lineShader;

        // font
        ::ddknd::asset::AssetID<::ddknd::asset::tag::Font> font;
    };

    struct DebugConfig
    {
        bool drawAxis = true;
        bool drawFps = true;
        bool drawSkeletons = true;

        DebugAxisStyle axisStyle{};
        DebugSkeletonStyle skeletonStyle{};
        DebugFPSTextStyle fpsStyle{};
    };

    struct DebugContext
    {
        const ::ddknd::system::FrameContext* frame = nullptr;
        ::ddknd::graphics::DebugDrawList* debugDraw = nullptr;
        const DebugConfig* config = nullptr;
        const DebugDrawResources* resources = nullptr;

        float fps = 0.0f;
    };

    class DebugSystemRunner
    {
      public:
        void Update(::ddknd::ecs::World& world, const DebugContext& ctx);
        void Submit(const DebugContext& ctx);

      private:
        void RunSkeletonDebug(::ddknd::ecs::World& world, const DebugContext& ctx);
        void RunFpsDebug(const DebugContext& ctx);
        void RunAxisDebug(const DebugContext& ctx);
    };
} // namespace ddknd::debug