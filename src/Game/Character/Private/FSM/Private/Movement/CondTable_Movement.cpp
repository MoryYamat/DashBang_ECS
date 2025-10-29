#include "Game/Character/Private/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Game/Character/Private/FSM/Private/Movement/MovementEnv.hpp"


namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;

	void BuildMovementCondTable_Stage(Engine::FSM::Core::CondTableStaging& stage)
	{
		stage.add("CanAndWantMove", [](const EnvSnapshot& e, const EvalCtx&)
			{
				const auto& env = static_cast<const MovementEnv&>(e);
				return env.canMove && env.wantMove;
			});

		stage.add("ShouldStop", [](const EnvSnapshot& e, const EvalCtx&)
			{
				const auto& env = static_cast<const MovementEnv&>(e);
				return (!env.canMove) || (!env.wantMove);
			});
	}
}