#pragma once

#include "Engine/ECS/EntityMgr.hpp"

#include "Game/Common/Logic/FSM/FSMRuntime.hpp"
#include "Game/Common/Logic/FSM/FSMStateComponent.hpp"
#include "Game/Common/Logic/FSM/Condition/ConditionRegistry.hpp"

#include "Common/EngineNamespaceDecl.h"

#include <typeindex>

namespace Game::Common::Logic::FSM
{
	template<typename FSMDef, typename Context>
	struct FSMSystem
	{
		using Axis = typename FSMDef::AxisType; // FSMの軸
		using StateComponent = FSMStateComponent<Axis>;
		using Registry = ConditionRegistry<Context>;

		// 毎フレーム状態を更新する
		void update(eNsECS::EntityMgr& ecs, const Registry& registry, const Context& ctx)
		{
			auto transitions =  buildRuntimeTransitions<FSMDef>(registry);

			for (auto entity : ecs.view<StateComponent>())
			{
				auto& state = ecs.get<StateComponent>(entity);

				for (const auto& t : transitions)
				{
					if (state.current == t.from && t.condition->evaluate(ctx))
					{
						state.current = t.to;
						break;
					}
				}
			}
		}
	};
}