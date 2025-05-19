// Actually process the SKillInstance based on the existing SkillInstance and the SkillDefinition of the corresponding SkillID
// 存在するSkillInstanceと該当のSkillIDのSkillDefinitionに基づいてSKillInstanceを実際に処理する

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputMapping.h"

#include "Graphics/Renderer/RenderContext.h"


#include "Game/SkillSystem/MasterData/SkillDatabase.h"

namespace SkillSystem::Casting
{
	// スキルの論理的形状（HitArea）を定義に従って生成する
	void SpawnSkillHitArea(ECS& ecs, SkillDatabase& skillDB);

	// 一時的に存在するスキル効果（攻撃範囲、エフェクトなど）の lifetime をカウントし、一定時間後に削除
	void SkillLifetimeSystem();
}