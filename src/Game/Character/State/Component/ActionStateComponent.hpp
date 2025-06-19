#pragma once 


namespace Game::Character::State::Action
{
	enum class ActionState
	{
		None,
		SkillCasting,// 複数フェーズあるが上層では一括で扱う
		Rolling, // 回避スキルとして扱う
		Guarding,// ガード
		UsingItem// アイテム使用
	};

	struct CharacterActionStateComponent
	{
		ActionState current = AcctionState::None;
		// ActionState previous = AcctionState::None;
	};
}