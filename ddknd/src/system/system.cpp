#include <ddknd/system/system.h>

#include <cassert>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/movement_component.h>


#include <ddknd/ecs/ecs.h>
#include <ddknd/graphics/gfx_asset_loader.h>

#include <ddknd/system/animator_system.h>
#include <ddknd/system/camera_system.h>
#include <ddknd/system/kinematic_system.h>
#include <ddknd/system/render_submit_system.h>
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
    void EngineSystemRunner::UpdateSimulation(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        RunKinematic(world,ctx);
        RunTransform(world,ctx);
    }
    void EngineSystemRunner::UpdateRenderPrepare(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        // camera
        RunCameraProjection(world, ctx);
        RunCameraMatrices(world, ctx);
        RunMainCameraExport(world, ctx);

        // rendering
        RunAnimator(world, ctx);
        RunSkinnedRenderSubmit(world, ctx);
    }
        
    void EngineSystemRunner::Update(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        //
        UpdateSimulation(world, ctx);
        UpdateRenderPrepare(world,ctx);
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
                                                 .require<component::TransformComponent, component::CameraLookComponent,
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

        for (auto [projection] : view)
        {
            CameraProjectionSystem::UpdateOne(projection, ctx.aspect);
        }
    }

    void EngineSystemRunner::RunAnimator(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        assert(ctx.graphicsAssetStore);
        assert(ctx.animationAssetStore);

        using namespace ecs;
        auto view = world.GetRegistry().view(
            query()
                .select<component::PoseComponent>()
                .require<component::AnimationPlaybackComponent, component::SkinnedModelComponent>());

        for (auto [pose, playback, skinnedModel] : view)
        {
            const auto* model = ctx.graphicsAssetStore->TryGet(skinnedModel.model);
            if (!model || !model->skeleton)
            {
                continue;
            }

            const auto* clip = ctx.animationAssetStore->TryGet(playback.state.clip);
            if (!clip)
            {
                continue;
            }

            ::ddknd::system::AnimationPlaybackSystem::UpdateOne(playback, pose, *model->skeleton, *clip, ctx.deltaTime);
        }
    }

    void EngineSystemRunner::RunSkinnedRenderSubmit(::ddknd::ecs::World& world,
                                                    const ::ddknd::system::FrameContext& ctx)
    {
        assert(ctx.graphicsAssetStore);
        assert(ctx.renderer);

        using namespace ecs;

        auto view = world.GetRegistry().view(
            query()
                .select<component::SkinnedModelComponent>()
                .require<component::MaterialComponent, component::TransformComponent, component::PoseComponent>());

        for (auto [model, material, transform, pose] : view)
        {
            SkinnedRenderSubmitSystem::UpdateOne(*ctx.renderer, model, material, transform, pose,
                                                 *ctx.graphicsAssetStore);
        }
    }

    void EngineSystemRunner::RunMainCameraExport(::ddknd::ecs::World& world, const ::ddknd::system::FrameContext& ctx)
    {
        assert(ctx.renderCamera);

        auto& out = *ctx.renderCamera;
        out.valid = false;

        auto view = world.GetRegistry().view(ecs::query()
                                                 .select<::ddknd::component::CameraMatricesComponent>()
                                                 .require<::ddknd::component::MainCameraTag>());

        int count = 0;
        for (auto [camera, mainCameraTag] : view)
        {
            (void)mainCameraTag;

            if (count == 0)
            {
                out.view = camera.view;
                out.proj = camera.proj;
                out.valid = true;
            }

            ++count;
        }

        assert(count <= 1 && "Multiple MainCameraTag components found");
        return;
    }
} // namespace ddknd::system