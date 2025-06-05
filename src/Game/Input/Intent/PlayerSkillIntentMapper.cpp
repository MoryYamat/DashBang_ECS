// 
#include "PlayerSkillIntentMapper.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Game/Input/InputActionComponent.h"

#include "Game/Combat/Skill/Component/SkillInputBindingComponent.h"
#include "Game/Combat/Skill/Intent/Component/SkillIntentComponent.h"

#include "Common/GameNamespaceDecl.h"

// ユーザの入力からPlayerCharacterのSkill用Intentを更新する(現在はPlayer専用だが後々AIも統合可能？)
void Game::Input::Intent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		gNsInput::InputActionComponent,
		gNsSkillComp::SkillInputBindingComponent,
		gNsSkillIntent::SkillIntentComponent>())
	{
		const auto& input = ecs.get<gNsInput::InputActionComponent>(e);
		const auto& binding = ecs.get<gNsSkillComp::SkillInputBindingComponent>(e);
		auto& intent = ecs.get<gNsSkillIntent::SkillIntentComponent>(e);

		intent.isActive = false;
		intent.requestedSlots.clear();

		for (const auto& [action, slot] : binding.actionToSlot)
		{
			if (input.isPressed(action))
			{
				intent.isActive = true;
				intent.requestedSlots.push_back(slot);
			}
		}
	}
}