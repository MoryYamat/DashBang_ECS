// 削除予定：FSM導入後廃止(同機能をもつ別ファイルを作成・)

// Actually process the SKillInstance based on the existing SkillInstance and the SkillDefinition of the corresponding SkillID
// 存在するSkillInstanceと該当のSkillIDのSkillDefinitionに基づいてSKillInstanceを実際に処理する

#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/Entity.h"



#include "Game/Collision/Private/Component/CollisionMaskComponent.h"

#include "Game/Combat/Private/Skill/MasterData/SkillDatabase.h"


namespace Game::Combat::Skill::System
{
	// スキル形状を定義に従って作成
	void spawnSkillHitArea(Engine::ECS::EntityMgr& ecs, Game::Combat::Skill::Data::SkillDatabase& skillDB, Engine::ECS::Entity skillEntity);

	// 廃止
	void SpawnSkillHitArea(Engine::ECS::EntityMgr& ecs);

	// スキルの論理的形状（HitArea）を定義に従って生成する
	void SpawnSkillHitArea(Engine::ECS::EntityMgr& ecs, Game::Combat::Skill::Data::SkillDatabase& skillDB);

	// 攻撃範囲形状のCollisionMaskを作成する
	Game::Collision::Component::CollisionMaskComponent generateSkillCollisionMask(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity caster
	);
	// void RotateVec2(const glm::vec2& v, float radians);

}