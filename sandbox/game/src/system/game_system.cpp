#include "game/system/game_system.h"

#include <cassert>

#include <ddknd/component/gfx_component.h>
#include <ddknd/component/movement_component.h>
#include <ddknd/ecs/ecs.h>
#include <ddknd/system/system.h>

#include "game/component/IntentComponent.h"
#include "game/component/animation_component.h"
#include "game/component/character_stats_component.h"
#include "game/component/controller_component.h"
#include "game/component/state_component.h"

#include "game/system/logic/game_animation_system.h"
#include "game/system/logic/game_camera_system.h"
#include "game/system/logic/game_movement_system.h"
#include "game/system/request/game_input_system.h"
#include "game/system/resolve/game_movement_intent_resolve_system.h"


namespace app::system
{

    void GameSystemRunner::UpdatePreEngine(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        if (ctx.paused)
        {
            return;
        }

        RunIntentRequest(world, ctx);
        RunState(world, ctx);
        RunIntentResolve(world, ctx);
        RunLogicPreEngine(world, ctx);
    }
    void GameSystemRunner::UpdatePostEngine(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        if (ctx.paused)
        {
            return;
        }

        RunLogicPostEngine(world, ctx);
    }

    // ============================== Layer System group ==============================
    void GameSystemRunner::RunIntentRequest(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunPlayerInput(world, ctx);
    }
    void GameSystemRunner::RunState(::ddknd::ecs::World& world, GameFrameContext& ctx) {}
    void GameSystemRunner::RunIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunMovementIntentResolve(world, ctx);
        // later:
        // RunCameraIntentResolve(world,ctx);
    }

    void GameSystemRunner::RunLogicPreEngine(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunMovement(world, ctx);
        RunPlayerLocomotionAnimation(world, ctx);
    }
    void GameSystemRunner::RunLogicPostEngine(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunCameraDesiredPose(world, ctx);
        RunCameraApply(world, ctx);
    }

    // ============================== Individual System group ==============================
    void GameSystemRunner::RunPlayerInput(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        assert(ctx.frame);
        assert(ctx.frame->actionInput);

        auto& input = *ctx.frame->actionInput;
        auto& reg = world.GetRegistry();

        auto move_view = reg.view(query()
                                      .select<component::RequestedMovementIntentComponent>()
                                      .require<component::PlayerControllerComponent>());

        for (auto [requested_intent, player] : move_view)
        {
            PlayerMovementIntentSystem::UpdateOne(requested_intent, input);
        }
    }

    void GameSystemRunner::RunPlayerLocomotionAnimation(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(
            query()
                .select<::ddknd::component::AnimationPlaybackComponent>()
                .require<component::PlayerLocomotionStateComponent, component::PlayerAnimationClipsComponent>());

        for (auto [playback, playerState, clips] : view)
        {
            PlayerAnimationSystem::UpdateOne(playback, playerState, clips);
        }
    }

    void GameSystemRunner::RunMovementIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view =
            reg.view(query()
                         .select<app::component::RequestedMovementIntentComponent>()
                         .require<app::component::PlayerControllerComponent, app::component::ControlledActorComponent,
                                  app::component::ControlledCameraRigComponent>());

        for (auto [request, controller, controlledActor, controlledCameraRig] : view)
        {
            (void)controller;
            auto* movementIntent = reg.TryGetComponent<app::component::MovementIntentComponent>(controlledActor.actor);
            if (!movementIntent)
            {
                continue;
            }

            auto* cameraOutput = reg.TryGetComponent<app::component::CameraOutputComponent>(controlledCameraRig.cameraRig);
            if(!cameraOutput)
            {
                continue;
            }
            auto* cameraLook = reg.TryGetComponent<::ddknd::component::CameraLookComponent>(cameraOutput->camera);
            if(!cameraLook)
            {
                continue;
            }

            MovementIntentResolveSystem::UpdateOne(*movementIntent, request, *cameraLook);
        }
    }

    void GameSystemRunner::RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(
            query()
                .select<::ddknd::component::VelocityComponent>()
                .require<app::component::MovementIntentComponent, app::component::CharacterMoveStatsComponent>());
        for (auto [velocity, intent, stats] : view)
        {
            MovementSystem::UpdateOne(velocity, intent, stats);
        }
    }

    void GameSystemRunner::RunCameraDesiredPose(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view =
            reg.view(query()
                         .select<app::component::CameraDesiredPoseComponent>()
                         .require<app::component::PlayerCameraRigComponent, app::component::CameraFollowTargetComponent,
                                  app::component::CameraOrbitComponent>());
        for (auto [desired, rig, follow, orbit] : view)
        {
            if (!rig.enabled)
            {
                desired.valid = false;
                continue;
            }

            auto* targetTransform =
                reg.TryGetComponent<::ddknd::component::TransformComponent>(follow.target); //@TODO lifetime management

            if (!targetTransform)
            {
                desired.valid = false;
                continue;
            }

            CameraDesiredPoseSystem::UpdateOne(desired, follow, orbit, *targetTransform);
        }
    }

    void GameSystemRunner::RunCameraApply(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query()
                                 .select<app::component::CameraDesiredPoseComponent>()
                                 .require<::app::component::CameraOutputComponent>());

        for (auto [desired, output] : view)
        {
            if (!desired.valid)
            {
                continue;
            }

            // @TODO lifetime management
            auto* cameraTransform = reg.TryGetComponent<::ddknd::component::TransformComponent>(output.camera);

            // @TODO lifetime management
            auto* cameraLook = reg.TryGetComponent<::ddknd::component::CameraLookComponent>(output.camera);

            if (!cameraTransform || !cameraLook)
            {
                continue;
            }

            CameraApplySystem::UpdateOne(*cameraTransform, *cameraLook, desired);
        }
    }
} // namespace app::system