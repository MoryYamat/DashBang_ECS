#include "Game/Character/FSM/Public/MovementAxisApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"
#include "Game/Character/Stats/Public/StatsComponent.hpp"
#include "Game/Character/Control/Public/IntentComponent.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::WorldSystem::Query;
	using namespace Engine::Component;

	// Entityのフィルタリング
	void BuildMovementPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, MovementPipeline& out)
	{
		out.clear();
		auto ents = ViewWhere(ctx.rw, All<MovementAxisComp, MovementStateComp, Velocity2DComponent>{});

		for (const auto& e : ents)
		{
			auto* axis = ctx.ww.TryGet<MovementAxisComp>(e);
			auto* state = ctx.ww.TryGet<MovementStateComp>(e);
			auto* vel = ctx.ww.TryGet<Engine::Component::Velocity2DComponent>(e);

			out.push_back(MovementPipelineEntry{
					e,
					axis,
					state,
					vel
				});
		}
	}

}