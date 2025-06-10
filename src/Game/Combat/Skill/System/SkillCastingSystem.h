// Actually process the SKillInstance based on the existing SkillInstance and the SkillDefinition of the corresponding SkillID
// 存在するSkillInstanceと該当のSkillIDのSkillDefinitionに基づいてSKillInstanceを実際に処理する

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"

#include "Game/Input/InputMapping.h"

#include "Engine/Graphics/Renderer/RenderContext.h"

#include "Game/Collision/Component/CollisionMaskComponent.h"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"

#include "Common/EngineNamespaceDecl.h"

namespace Game::Combat::Skill::System
{
	// スキル形状を定義に従って作成
	void spawnSkillHitArea(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB, eNsECS::Entity skillEntity);

	// 廃止
	void SpawnSkillHitArea(eNsECS::EntityMgr& ecs);

	// スキルの論理的形状（HitArea）を定義に従って生成する
	void SpawnSkillHitArea(eNsECS::EntityMgr& ecs, gNsSkillData::SkillDatabase& skillDB);

	// 攻撃範囲形状のCollisionMaskを作成する
	gNsCollComp::CollisionMaskComponent generateSkillCollisionMask(
		eNsECS::EntityMgr& ecs,
		eNsECS::Entity caster
	);
	// void RotateVec2(const glm::vec2& v, float radians);
}