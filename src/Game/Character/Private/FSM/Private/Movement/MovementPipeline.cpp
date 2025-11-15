#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisComponent.hpp"


namespace Game::Character::FSM::Movement
{
	void BuildMovementPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, MovementPipeline& out)
	{
		out.clear();
		ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e)
			{
				auto* axis = ctx.ww.TryGet<MovementAxisComp>(e);
				auto* state = ctx.ww.TryGet<MovementStateComp>(e);
				if (!axis || !state) return;


				out.push_back(MovementPipelineEntry{
					e,
					axis,
					state
					});
			});
	}
}