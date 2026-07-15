#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/ecs/entity/entity.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/system/system_fwd.h>

#include <ddknd/asset/asset_id.h>
#include <ddknd/asset/asset_tag.h>
#include <ddknd/math/math.h>

namespace ddknd::debug
{
    struct DebugFramePerformanceInfoTextStyle
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

    struct DebugHitboxStyle
    {
      ddknd::math::Vec4f hitboxColor{1.0f, 0.0f, 0.0f, 1.0f};
      ddknd::math::Vec4f hurtboxColor{0.0f, 1.0f, 0.0f, 1.0f};
    };

    struct DebugHitEventLine
    {
      ddknd::ecs::Entity owner;
      ddknd::ecs::Entity hitbox;
      ddknd::ecs::Entity target;
      float remaining = 0.5f;
    };

    struct FramePerformanceInfo
    {
      float currentMs = 0.0f;
      float p90Ms = 0.0f;
      float p99Ms = 0.0f;
      float fps = 0.0f;         // per 0.5s
    };

    enum class CameraOverrideMode
    {
        None,       // Use normal app/game camera for rendering
        DebugCamera // Use debug camera view/proj for rendering
    };

    struct DebugCameraConfig
    {
        CameraOverrideMode overrideMode = CameraOverrideMode::None;

        bool consumeInput = true;
        float moveSpeed = 10.0f;
        float lookSensitivity = 0.1f;
    };

    struct DebugConfig
    {
        bool drawAxis = true;
        bool drawFrameTimeInfo = true;
        bool drawSkeletons = true;
        bool drawHitboxes = true;
        bool drawHurtboxes = true;
        bool drawHitEvents = true;

        DebugAxisStyle axisStyle{};
        DebugSkeletonStyle skeletonStyle{};
        DebugFramePerformanceInfoTextStyle framePerformanceStyle{};
        DebugHitboxStyle hitboxStyle{};

        DebugCameraConfig camera{};
    };

    struct DebugContext
    {
        const ::ddknd::system::FrameContext* frame = nullptr;
        graphics::debug::DebugDrawList* debugDraw = nullptr;
        const DebugConfig* config = nullptr;
        const DebugDrawResources* resources = nullptr;

        //float fps = 0.0f;
        FramePerformanceInfo framePerformance{};
    };

    class DebugSystemRunner
    {
      public:
        void BeginFrame(const DebugContext& ctx);
        void EndFrame(const DebugContext& ctx);

        void Update(::ddknd::ecs::World& world, const DebugContext& ctx);
        void Submit(const DebugContext& ctx);

      private:
        void RunSkeletonDebug(::ddknd::ecs::World& world, const DebugContext& ctx);
        void RunFramePerfDebug(const DebugContext& ctx);
        void RunAxisDebug(const DebugContext& ctx);

        void RunHitboxDebug(ddknd::ecs::World& world, const DebugContext& ctx);
        void RunHurtboxDebug(ddknd::ecs::World& world, const DebugContext& ctx);

        bool PrepareTextDebug(const DebugContext& ctx);

        void RunHitEventDebug(const DebugContext& ctx);

        std::vector<DebugHitEventLine> hitEventLines_;
    };
} // namespace ddknd::debug