#include "BuildCCAnimationQuerySystem.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Game/Character/FSM/CC/StateModel/CCStateComponent.hpp"
#include "Game/Character/Animation/Query/CC/CCAnimationQueryComponent.hpp"

namespace Game::Character::Animation::Query::CC
{
	namespace Ops = Engine::ECS::Ops;
	namespace StateModel = Game::Character::FSM::CC::StateModel;

	using CCQuery = Game::Character::Animation::Query::CC::CCAnimationQueryComponent;
	using CCState = Game::Character::FSM::CC::StateModel::CCStateComponent;


	void BuilCCAnimationQuerySystem(Engine::ECS::EntityMgr& ecs)
	{
		for (auto e : ecs.view<
			CCState,
			CCQuery
			>())
		{
			const auto& state = Ops::Get<CCState>(ecs, e);
			auto& q = Ops::Get<CCQuery>(ecs, e);

			// FIXME: causeid -> std::optional よって assertionでエラーになるこの代入は危険
			q.current = state.current;
			// q.causeId = *state.applied.causeId;// 必要か要検討
			q.appliedAt = state.applied.appliedAt;
			q.isCCActive = StateModel::isCC(state.current);
			q.valid = true;

		}
	}
}