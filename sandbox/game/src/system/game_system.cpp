#include "game/system/game_system.h"

#include <cassert>

#include <ddknd/ecs/ecs.h>
#include <ddknd/component/movement_component.h>
#include <ddknd/system/system.h>


#include "game/component/IntentComponent.h"
#include "game/component/character_stats_component.h"

#include "game/system/input_system.h"
#include "game/system/movement_system.h"


namespace app::system
{
    void GameSystemRunner::Update(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        if(ctx.paused)
        {
            return;
        }

        RunIntentRequest(world,ctx);
        RunState(world,ctx);
        RunIntentResolve(world,ctx);
        RunLogic(world,ctx);
    }

    // ============================== Layer System group ============================== 
    void GameSystemRunner::RunIntentRequest(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunPlayerInput(world,ctx);
    }
    void GameSystemRunner::RunState(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {

    }
    void GameSystemRunner::RunIntentResolve(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {

    }
    void GameSystemRunner::RunLogic(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        RunMovement(world,ctx);
    }


    // ============================== Individual System group ============================== 
    void GameSystemRunner::RunPlayerInput(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        assert(ctx.frame);
        assert(ctx.frame->actionInput);

        auto& input = *ctx.frame->actionInput;
        auto& reg = world.GetRegistry();

        auto move_view = reg.view(query().select<component::RequestedMovementIntentComponent>()
                                .require<component::PlayerControlComponent>());

        for(auto [requested_intent, player] : move_view)
        {
            PlayerMovementIntentSystem::UpdateOne(requested_intent,input);
        }
    }
    void GameSystemRunner::RunPlayerAnimation(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
    }

    
    void GameSystemRunner::RunMovement(::ddknd::ecs::World& world, GameFrameContext& ctx)
    {
        using namespace ::ddknd::ecs;

        auto& reg = world.GetRegistry();

        auto view = reg.view(query().select<::ddknd::component::VelocityComponent>()
                                    .require<app::component::MovementIntentComponent,
                                            app::component::CharacterMoveStatsComponent>());
        for(auto[velocity, intent, stats]: view)
        {
            MovementSystem::UpdateOne(velocity,intent,stats);
        }
    }
}// namespace app::system