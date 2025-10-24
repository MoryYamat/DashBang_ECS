// SkillExecutionComponentが特定のphaseに到達したときに，攻撃判定を生成するシステム


#pragma once

#include "Engine/ECS/EntityManager.h"



// 削除予定：FSM導入後廃止
namespace Game::Combat::Skill::System
{
	void SpawnSkillHitAreas(Engine::ECS::EntityMgr& ecs);
}