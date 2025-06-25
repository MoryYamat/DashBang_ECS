#include "CharacterSkillIntentResolver.hpp"

#include "Game/Character/Control/Skill/Intent/SkillIntentComponent.hpp"
#include "Game/Combat/Skill/Component/SkillSlotAssignmentComponent.h"
#include "Game/Character/State/Component/Action/ActionStateComponent.hpp"

#include "Common/GameNamespaceDecl.h"

// 効率問題：すぐには問題にならないが、将来的にキャッシュを検討する
// キャラクターのスキルの意図を反映するかしないか判定するシステム
void Game::Character::Control::Skill::UpdateCharacterSkillIntentResovlver(eNsECS::EntityMgr& ecs)
{
	for (eNsECS::Entity e : ecs.view<
		gNsCharacterControlSkill::SkillIntentComponent,
		gNsSkillComp::SkillSlotAssignmentComponent,
		gNsCharacterState::Action::CharacterActionStateComponent>())
	{
		auto& intent = ecs.get<gNsCharacterControlSkill::SkillIntentComponent>(e);
		const auto& state = ecs.get<gNsCharacterState::Action::CharacterActionStateComponent>(e);

		// 仮判定：スキル発動できる状態でなければintent を無効か
		if (state.current == gNsCharaActionState::ActionState::None)
		{
			// 発動可能状態ならintentはそのまま
		}
		else
		{
			// スキル発動可能状態ならintent を無効化
			intent.isActive = true;
			intent.requestedSlots.clear();
		}
	}
}