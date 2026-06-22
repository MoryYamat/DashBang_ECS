#pragma once

#include <ddknd/input/input_fwd.h>
#include <ddknd/system/system_fwd.h>
#include <ddknd/ecs/ecs_fwd.h>

namespace app::system
{
    struct GameFrameContext
    {
        const ::ddknd::system::FrameContext* frame = nullptr;
        const ::ddknd::input::ActionInputSystem* input = nullptr;
        bool paused = false;
    };

    class GameSystemRunner
    {
        public:
            void UpdatePreEngine(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void UpdatePostEngine(::ddknd::ecs::World& world, GameFrameContext& ctx);
        private:
            void RunIntentRequest(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunState(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunLogicPreEngine(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunLogicPostEngine(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunPlayerInput(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerLocomotionAnimation(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerAttackAnimation(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunMovementIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerCameraInput(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunCharacterFacing(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerLocomotionState(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunPlayerAttackIntent(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerAttackIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerAttackState(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunAttack(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunCharacterModfier(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunHitboxSpawnSystem(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunHitboxLifeTimeSystem(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunCameraDesiredPose(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunCameraApply(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunCameraIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);
    };
}