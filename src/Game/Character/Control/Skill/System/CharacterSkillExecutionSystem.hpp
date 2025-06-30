#pragma once

#include "Engine/ECS/EntityManager.h"


#include "Engine/ECS/Component/Logic2D/Logic2DTransformComponent.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Character::Control::Skill
{
	// キャラクターのスキル実行システム
	void UpdateCharacterSkillExecution(eNsECS::EntityMgr& ecs, float deltaTime);

	// スキルインスタンスを作成し，コンテキスト情報を設定する
	eNsECS::Entity CreateSkillInstanceWithContext
	(
		eNsECS::EntityMgr& ecs,
		eNsECS::Entity caster,
		int skillId,
		const eNsLogic2DComp::Logic2DTransformComponent& casterTransform
	);
}