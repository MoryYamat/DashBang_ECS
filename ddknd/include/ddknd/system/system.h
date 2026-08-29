#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/input/input_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/event/event_fwd.h>

#include <ddknd/ui/ui_context.h>

namespace ddknd::system
{
    // Execution environment in that frame
    struct FrameContext
    {
        float deltaTime;

        ::ddknd::input::ActionInputSystem* actionInput = nullptr;
        ddknd::input::DeviceInput* deviceInput = nullptr;

        // camera
        float aspect;

        // asset references
        ddknd::graphics::GraphicsAssetStore* graphicsAssetStore = nullptr;
        ddknd::animation::AnimationAssetStore* animationAssetStore = nullptr;

        // renderer
        ::ddknd::graphics::RendererSystem* renderer = nullptr;
        ::ddknd::graphics::RenderCamera* renderCamera = nullptr;


        // ui
        ddknd::ui::UIContext* uiContext = nullptr;

        // temporary per-frame event bufers
        // @TODO: replace with generic EvenetManager / EvenetQueue / ResourceStorage
        ddknd::event::HitboxHitEventBuffer* hitboxHitEvents = nullptr;
    };

    /*
    * 
    */
    class EngineSystemRunner
    {
      public:
        void UpdateSimulation(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void UpdateRenderPrepare(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void Update(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

      private:
        void RunTransform(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunKinematic(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunCameraMatrices(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunCameraProjection(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunAnimationPlayback(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunAnimationPoseSampling(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);


        // rendering
        void RunMeshRenderSubmit(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunSkinnedRenderSubmit(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunMainCameraExport(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

        // entity lifetime
        void RunLifetimeSystem(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

  
        void RunHitboxCollisionSystem(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

        void RunHitTest(ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunUIEvent(ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunUIReset(ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
    };
} // namespace ddknd::system