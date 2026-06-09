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
            void Update(::ddknd::ecs::World& world, GameFrameContext& ctx);
        private:
            void RunIntentRequest(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunState(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunLogic(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunPlayerInput(::ddknd::ecs::World& world, GameFrameContext& ctx);
            void RunPlayerAnimation(::ddknd::ecs::World& world, GameFrameContext& ctx);

            void RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx);
    };
}