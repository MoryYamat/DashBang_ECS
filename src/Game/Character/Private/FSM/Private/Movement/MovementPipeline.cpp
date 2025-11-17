#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/Character/Private/FSM/Public/MovementAxisComponent.hpp"
#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"
#include "Game/Character/Private/Stats/Public/StatsComponent.hpp"
#include "Game/Character/Private/Control/Movement/Component/IntentComponent.hpp"

namespace Game::Character::FSM::Movement
{
	// Entityのフィルタリング
	void BuildMovementPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, MovementPipeline& out)
	{
		out.clear();
		ctx.ww.ForEachAlive([&](Engine::ECS::Core::Entity e)
			{
				// FSM内で必ず持つ必要があるComponentの設定
				// 最小公倍数であるべき
				auto* axis = ctx.ww.TryGet<MovementAxisComp>(e);
				auto* state = ctx.ww.TryGet<MovementStateComp>(e);
				auto* vel = ctx.ww.TryGet<Engine::Component::Velocity2DComponent>(e);
				// auto* stats = ctx.ww.TryGet<Game::Character::Stats::CharacterStatsComponent>(e);
				// auto* intent = ctx.ww.TryGet<Game::Character::Control::Movement::MovingIntentComponent>(e);
				if (!axis || !state || !vel) return;


				out.push_back(MovementPipelineEntry{
					e,
					axis,
					state,
					vel
					});
			});
	}
}