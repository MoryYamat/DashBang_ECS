#include <ddknd/system/system.h>

#include <cassert>

#include <ddknd/component/gfx_component.h>
#include <ddknd/ecs/ecs.h>

#include <ddknd/component/movement_component.h>
#include <ddknd/system/camera_system.h>
#include <ddknd/system/kinematic_system.h>
#include <ddknd/system/transform_system.h>


// Argument Order Policy
// System::UpdateOne(
//     WriteComponent& write,
//     const ReadComponentA& readA,
//     const ReadComponentB& readB,
//     ...
//     const FrameValue& value
// );
// Keep the number of writes to 1 whenever possible.

namespace ddknd::system
{
    void EngineSystemRunner::Update(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        // 
        RunKinematic(world, ctx);
        RunTransform(world, ctx);
        
        // camera
        RunCameraProjection(world,ctx);
        RunCameraMatrices(world,ctx);
    }

    void EngineSystemRunner::RunTransform(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        using namespace ecs;
        auto view = world.GetRegistry().view(query().select<component::TransformComponent>());

        for (auto [transform] : view)
        {
            TransformSystem::UpdateOne(transform);
        }
    }

    void EngineSystemRunner::RunKinematic(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        using namespace ecs;
        // need to consideration
        auto view = world.GetRegistry().view(
            query().select<component::VelocityComponent>().require<component::TransformComponent>());

        for (auto [velocity, transform] : view)
        {
            KinematicSystem::UpdateOne(transform, velocity, ctx.deltaTime);
        }
    }

    void EngineSystemRunner::RunCameraMatrices(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        using namespace ecs;
        auto view = world.GetRegistry().view(query()
                                                 .select<component::CameraMatricesComponent>()
                                                 .require<component::TransformComponent, 
                                                          component::CameraLookComponent,
                                                          component::CameraProjectionComponent>());

        for (auto [matrices, transform, look, projection] : view)
        {
            CameraMatricesSystem::UpdateOne(matrices, transform, look, projection);
        }
    }

    void EngineSystemRunner::RunCameraProjection(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        using namespace ecs;
        auto view = world.GetRegistry().view(query().select<component::CameraProjectionComponent>());

        for(auto [projection] : view)
        {
            CameraProjectionSystem::UpdateOne(projection, ctx.aspect);
        }
    }

    void EngineSystemRunner::RunAnimator(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        assert(ctx.graphicsAssetStore);
        assert(ctx.animationAssetStore);

        using namespace ecs;
        auto view = world.GetRegistry().view(query().select<component::PoseComponent>()
                                                    .require<component::AnimationPlaybackComponent, component::SkinnedModelComponent>());

        for(auto [pose, playabck, skinnedModel] : view)
        {
            // AnimatorSystem::UpdateOne(animator);
        }
    }
} // namespace ddknd::system