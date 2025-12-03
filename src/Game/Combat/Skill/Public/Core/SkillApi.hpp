

#pragma once
#include "Game/Character/Control/Public/ControlFwd.hpp"
#include "Game/Character/FSM/Public/FSMFwd.hpp"
#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <glm/glm.hpp>
#include <vector>
#include <span>

namespace Game::Combat::Skill
{
	// private
	void RegisterSkillDef(SkillCatalogBuilder& builder);



	


	// public
	void InitAllSkillSystem(Engine::WorldSystem::Core::WorldCtx& ctx);

	bool InitSkillSlot(
		Engine::WorldSystem::Core::WorldCtx& ctx,
		SkillSlotLoadoutDTO& dto,
		SkillSlotComponent& outSlots
	);

	bool InitSkillSlot(Engine::WorldSystem::Core::WorldCtx& ctx);

}