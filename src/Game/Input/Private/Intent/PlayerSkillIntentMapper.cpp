// 
#include "PlayerSkillIntentMapper.h"


#include "Game/Input/Public/InputActionComponent.hpp"

#include "Game/Combat/Private/Skill/Component/SkillInputBindingComponent.h"
#include "Game/Combat/Private/Skill/Intent/Component/SkillIntentComponent.h"

#include "Game/ECS/Public/CharacterAttribTags.h"

#include "Game/Character/Private/Control/Skill/Intent/SkillIntentComponent.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include <iostream>

// ユーザの入力からPlayerCharacterのSkill用Intentを更新する(現在はPlayer専用だが後々AIも統合可能？)
// 純粋なInput->Intentの変換を行うシステム
void Game::Input::Intent::SkillIntentMappingSystem::UpdatePlayerSkillIntent(Engine::ECS::EntityMgr& ecs)
{
	// std::cout << "Here\n";
	for (Engine::ECS::Entity e : ecs.view<
		Game::Input::InputActionComponent,
		Game::Combat::Skill::Component::SkillInputBindingComponent,
		Game::Character::Control::Skill::SkillIntentComponent,
		Game::ECS::Tags::PlayerCharacterTag>())
	{
		// ここまで来てない
		const auto& input = ecs.get<Game::Input::InputActionComponent>(e);
		const auto& binding = ecs.get<Game::Combat::Skill::Component::SkillInputBindingComponent>(e);
		auto& intent = ecs.get<Game::Character::Control::Skill::SkillIntentComponent>(e);

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

namespace Game::Input::Intent
{
	void SkillIntentMappingSystem::UpdatePlayerSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx)
	{

	}
}