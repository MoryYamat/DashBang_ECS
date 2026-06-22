#pragma once

#include <ddknd/ecs/ecs_fwd.h>
#include <ddknd/input/input_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>
#include <ddknd/event/event_fwd.h>


// ================================ Forward Declarations ================================
namespace ddknd::graphics
{
    class GraphicsAssetStore;
}// namespace ddknd::graphics

namespace ddknd::animation
{
    class AnimationAssetStore;
}
// ======================================================================================

namespace ddknd::system
{
    // Execution environment in that frame
    struct FrameContext
    {
        // time
        float deltaTime;

        ::ddknd::input::ActionInputSystem* actionInput = nullptr;

        // camera
        float aspect;

        // asset references
        ddknd::graphics::GraphicsAssetStore* graphicsAssetStore = nullptr;
        ddknd::animation::AnimationAssetStore* animationAssetStore = nullptr;

        // renderer
        ::ddknd::graphics::RendererSystem* renderer = nullptr;
        ::ddknd::graphics::RenderCamera* renderCamera = nullptr;

        // temporary per-frame event bufers
        // @TODO: replace with generic EvenetManager / EvenetQueue / ResourceStorage
        ddknd::event::HitboxHitEventBuffer* hitboxHitEvents = nullptr;
    };

    // Scheduler
    // topological sort
    // parallelism
    //
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
        void RunAnimator(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

        // rendering
        void RunSkinnedRenderSubmit(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
        void RunMainCameraExport(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

        void RunLifetimeSystem(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);

        
        void RunHitboxCollisionSystem(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx);
    };
} // namespace ddknd::system

// API Image
// auto view = registry.view(.query().select<AComp>().require<BComp>());
// view.each([](Acomp& a, Bcomp& b){ABSystem::Updateone(a,b);});
