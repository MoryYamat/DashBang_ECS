// 各キャラクタ( Player / NPC) がどのスキルを持っているかを定義するコンポーネント
// つまり所持リスト
#pragma once

#include <unordered_set>

// 保持し得る全スキルのリスト
struct SkillSetComponent
{
	std::unordered_set<int> ownedSkillIds;
};