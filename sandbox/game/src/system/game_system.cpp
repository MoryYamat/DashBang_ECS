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
#include "game/system/logic/game_character_facing_system.h"
#include "game/system/logic/game_hitbox_logic_system.h"
#include "game/system/logic/game_movement_system.h"
#include "game/system/request/game_input_system.h"
#include "game/system/resolve/game_movement_intent_resolve_system.h"
#include "game/system/state/game_character_state_system.h"

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
        RunPlayerAttackIntent(world, ctx);
        RunCameraIntentResolve(world, ctx);
    }
    void GameSystemRunner::RunState(::ddknd::ecs::World& world, GameFrameContext& ctx) {}
    void GameSystemRunner::RunIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunMovementIntentResolve(world, ctx);
        RunPlayerCameraInput(world, ctx);
        // later:
        // RunCameraIntentResolve(world,ctx);
        RunPlayerAttackIntentResolve(world, ctx);

        // state
        RunPlayerLocomotionState(world, ctx);
        RunPlayerAttackState(world, ctx);
    }

    void GameSystemRunner::RunLogicPreEngine(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunCharacterModfier(world, ctx);

        RunMovement(world, ctx);
        RunCharacterFacing(world, ctx);

        // hitbox
        RunHitboxSpawnSystem(world, ctx);

        // chose animation
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

        auto view = reg.view(query()
                                 .select<::ddknd::component::AnimationPlaybackComponent>()
                                 .require<component::PlayerLocomotionStateComponent, component::AttackStateComponent,
                                          component::PlayerAnimationClipsComponent>());

        for (auto [playback, moveState, attackState, clips] : view)
        {
            PlayerAnimationSystem::UpdateOne(playback, moveState, attackState, clips);
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

            auto* cameraOutput =
                reg.TryGetComponent<app::component::CameraOutputComponent>(controlledCameraRig.cameraRig);
            if (!cameraOutput)
            {
                continue;
            }
            auto* cameraLook = reg.TryGetComponent<::ddknd::component::CameraLookComponent>(cameraOutput->camera);
            if (!cameraLook)
            {
                continue;
            }

            MovementIntentResolveSystem::UpdateOne(*movementIntent, request, *cameraLook);
        }
    }

    void GameSystemRunner::RunPlayerCameraInput(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query()
                                 .select<app::component::RequestedCameraIntentComponent>()
                                 .require<app::component::PlayerControllerComponent,
                                          app::component::CameraControllerSettingsComponent>());

        for (auto [request, controller, settings] : view)
        {
            (void)controller;

            PlayerCameraIntentSystem::UpdateOne(request, settings, *ctx.frame->actionInput);
        }
    }

    void GameSystemRunner::RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view =
            reg.view(query()
                         .select<::ddknd::component::VelocityComponent>()
                         .require<app::component::MovementIntentComponent, app::component::CharacterMoveStatsComponent,
                                  app::component::CharacterControlModifierComponent>());
        for (auto [velocity, intent, stats, modifier] : view)
        {
            MovementSystem::UpdateOne(velocity, intent, stats, modifier);
        }
    }

    void GameSystemRunner::RunCharacterFacing(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(
            query()
                .select<::ddknd::component::TransformComponent>()
                .require<app::component::MovementIntentComponent, app::component::CharacterControlModifierComponent>());

        for (auto [transform, moveIntent, modifier] : view)
        {
            CharacterFacingSystem::UpdateOne(transform, moveIntent, modifier);
        }
    }

    void GameSystemRunner::RunPlayerLocomotionState(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query()
                                 .select<app::component::PlayerLocomotionStateComponent>()
                                 .require<app::component::MovementIntentComponent>());

        for (auto [state, moveIntent] : view)
        {
            PlayerLocomotionStateSystem::UpdateOne(state, moveIntent);
        }
    }

    void GameSystemRunner::RunPlayerAttackIntent(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        assert(ctx.frame);
        assert(ctx.frame->actionInput);

        auto& input = *ctx.frame->actionInput;
        auto& reg = world.GetRegistry();

        auto view = reg.view(query()
                                 .select<app::component::RequestedAttackIntentComponent>()
                                 .require<app::component::PlayerControllerComponent>());

        for (auto [requested_intent, controller] : view)
        {
            (void)controller;
            if (!requested_intent.enabled)
                continue;
            PlayerAttackIntentSystem::UpdateOne(requested_intent, input);
        }
    }

    void GameSystemRunner::RunPlayerAttackIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        assert(ctx.frame);

        auto& reg = world.GetRegistry();

        auto view = reg.view(
            query()
                .select<app::component::RequestedAttackIntentComponent>()
                .require<app::component::PlayerControllerComponent, app::component::ControlledActorComponent>());

        for (auto [request, controller, controlled] : view)
        {
            (void)controller;
            auto* intent = reg.TryGetComponent<app::component::AttackIntentComponent>(controlled.actor);
            if (!intent)
                continue;

            AttackIntentResolveSystem::UpdateOne(*intent, request);
        }
    }

    void GameSystemRunner::RunPlayerAttackState(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ddknd::ecs;

        assert(ctx.frame);

        auto& reg = world.GetRegistry();
        auto view = reg.view(query()
                                 .select<app::component::AttackStateComponent>()
                                 .require<app::component::AttackIntentComponent, app::component::AttackDefComponent>());
        for (auto [state, intent, definition] : view)
        {
            PlayerAttackStateSystem::UpdateOne(state, intent, definition, ctx.frame->deltaTime);
        }
    }

    void GameSystemRunner::RunCharacterModfier(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ddknd::ecs;

        auto& reg = world.GetRegistry();
        auto view = reg.view(query()
                                 .select<app::component::CharacterControlModifierComponent>()
                                 .require<app::component::AttackStateComponent, app::component::AttackDefComponent>());

        for (auto [modifier, state, def] : view)
        {
            AttackControlModifierSystem::UpdateOne(modifier, state, def);
        }
    }

    void GameSystemRunner::RunHitboxSpawnSystem(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        (void)ctx;
        app::system::AttackHitboxSpawnSystem::Update(world);
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

    void GameSystemRunner::RunCameraIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(
            query()
                .select<app::component::RequestedCameraIntentComponent>()
                .require<app::component::PlayerControllerComponent, app::component::ControlledCameraRigComponent>());

        for (auto [request, controller, controlledCameraRig] : view)
        {
            (void)controller;

            auto* orbit = reg.TryGetComponent<app::component::CameraOrbitComponent>(controlledCameraRig.cameraRig);
            if (!orbit)
                continue;

            CameraIntentResolveSystem::UpdateOne(*orbit, request);
        }
    }
} // namespace app::system