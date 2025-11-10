//#include "Game/Character/Private/FSM/Public/FSMApi.hpp"
//
//#include "Engine/FSM/Public/Core/Types.hpp"
//#include "Game/Character/Private/FSM/Private/Movement/MovementEnv.hpp"
//
//
//namespace Game::Character::FSM::Movement
//{
//	using namespace Engine::FSM::Core;
//
//	void BuildMovementCondTable_Stage(Engine::FSM::Core::CondTableStaging& stage)
//	{
//		stage.add("CanAndWantMove", [](const EnvSnapshot& e, const EvalCtx&)
//			{
//				const auto& env = static_cast<const MovementEnv&>(e);
//				return env.canMove && env.wantMove;
//			});
//
//		stage.add("ShouldStop", [](const EnvSnapshot& e, const EvalCtx&)
//			{
//				const auto& env = static_cast<const MovementEnv&>(e);
//				return (!env.canMove) || (!env.wantMove);
//			});
//	}
//}

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Game/Character/Private/FSM/Public/FSMApi.hpp"
#include "MovementEnv.hpp"

using namespace Engine::FSM::Core;
using namespace Game::Character::FSM::Movement;

static bool Cond_CanMove(const EnvSnapshot& base, const EvalCtx&)
{
	auto& env = static_cast<const MovementEnv&>(base);
	return env.hasMoveInput && env.canMove;
}

static bool Cond_ShouldStop(const EnvSnapshot& base, const EvalCtx&)
{
	auto& env = static_cast<const MovementEnv&>(base);
	return !env.hasMoveInput || !env.canMove;
}

//static constexpr NamedCondBinding kMovementBindings[] =
//{
//	{ "canMove", &Cond_CanMove},
//	{"shouldStop", &Cond_ShouldStop},
//};
//
//
//namespace Game::Character::FSM::Movement
//{
//	void ProvideMovementConds(const Engine::FSM::Core::FSMCatalog& cat,
//		std::vector<Engine::FSM::Core::AxisCondBindings>& out)
//	{
//		out.push_back(AxisCondBindings{
//			"Movement",
//			std::span<const NamedCondBinding>{kMovementBindings, std::size(kMovementBindings)}
//			});
//	}
//}
