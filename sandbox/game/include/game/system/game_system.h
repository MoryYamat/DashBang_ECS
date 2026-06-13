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

            void RunMovementIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunCameraDesiredPose(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunCameraApply(::ddknd::ecs::World& world, GameFrameContext& ctx);
    };
}