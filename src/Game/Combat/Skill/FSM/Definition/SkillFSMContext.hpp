// ContextはFSMの「評価・制御」に必要なすべての実行情報を1つに集約する構造


#pragma once

#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include <cstdint>

namespace Game::Combat::Skill::FSM
{

	// FSMの評価・更新中に使われる「一時的かつ変化しうる実行時データの束」
	struct SkillFSMContext
	{
		uint32_t id = 0;
		float elapsedTime = 0.0f;// スキルの経過時間
		bool isInterrupted = false; // スキルが中断されたかどうか
	};
}