//
#include "Game/Character/Private/FSM/Private/Movement/Public/MovementAxisComponent.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/InitAxisComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

namespace Game::Character::FSM::Movement
{
	using namespace Engine::FSM::Core;
	using namespace Engine::WorldSystem::Core;

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp)
	{

		return InitAxisComponent<MovementTag>(ctx, comp);
	}

	bool InitMovementAxis(WorldCtx& ctx, MovementAxisComp& comp, std::string_view fsmName)
	{
		return InitAxisComponent<MovementTag>(ctx, comp, fsmName);
	}
}