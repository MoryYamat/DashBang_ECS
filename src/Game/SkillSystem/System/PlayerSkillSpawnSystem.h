// A system that generates player character skills based on input
// 入力をもとにプレイヤーキャラクターのスキルを生成するシステム

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Game/Input/InputState.h"

namespace SkillSystem
{
	void PlayerSkillSpawnSystem(ECS& ecs, InputState& input);
}