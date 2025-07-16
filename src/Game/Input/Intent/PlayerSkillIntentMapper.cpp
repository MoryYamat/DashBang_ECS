// 
#include "PlayerSkillIntentMapper.h"

#include "Engine/ECS/Component/Tags/PlayerCharacterTag.h"

#include "Game/Input/InputActionComponent.h"

#include "Game/Combat/Skill/Component/SkillInputBindingComponent.h"
#include "Game/Combat/Skill/Intent/Component/SkillIntentComponent.h"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Common/GameNamespaceDecl.h"

#include <iostream>

// ユーザの入力からPlayerCharacterのSkill用Intentを更新する(現在はPlayer専用だが後々AIも統合可能？)
// 純粋なInput->Intentの変換を行うシステム
void Game::Input::Intent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(eNsECS::EntityMgr& ecs)
{
	// std::cout << "Here\n";
	for (eNsECS::Entity e : ecs.view<
		gNsInput::InputActionComponent,
		gNsSkillComp::SkillInputBindingComponent,
		gNsCharacterControlSkill::SkillIntentComponent,
		gNsTags::PlayerCharacterTag>())
	{
		// ここまで来てない
		const auto& input = ecs.get<gNsInput::InputActionComponent>(e);
		const auto& binding = ecs.get<gNsSkillComp::SkillInputBindingComponent>(e);
		auto& intent = ecs.get<gNsCharacterControlSkill::SkillIntentComponent>(e);

		intent.isActive = false;
		intent.requestedSlots.clear();


		for (const auto& [action, slot] : binding.actionToSlot)
		{
			if (input.isJustPressed(action))
			{
				// ボタンが押されている場合，trueに
				intent.isActive = true;
				intent.requestedSlots.push_back(slot);
				// std::cout << "[SkillIntent] Intent triggered." << std::endl;
				// std::cout << "[SkillIntent] Slot triggered: " << static_cast<int>(slot) << std::endl;
			}
		}
	}
}