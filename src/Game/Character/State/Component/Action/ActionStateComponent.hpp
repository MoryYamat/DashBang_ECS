#pragma once 


namespace Game::Character::State::Action
{
	enum class ActionState
	{
		None,
		SkillCasting,// スキル使用：複数フェーズあるが上層では一括で扱う
		//Rolling, // 回避スキルとして扱う
		//Guarding,// ガード
		UsingItem// アイテム使用
	};

	struct CharacterActionStateComponent
	{
		ActionState current = ActionState::None;
		// ActionState previous = AcctionState::None;
	};

	inline const char* ToString(ActionState state)
	{
		switch (state)
		{
		case Game::Character::State::Action::ActionState::None:
			return "None";
		case Game::Character::State::Action::ActionState::SkillCasting:
			return "SkillCasting";
		//case Game::Character::State::Action::ActionState::Rolling:
		//	return "Rolling";
		//case Game::Character::State::Action::ActionState::Guarding:
		//	return "Guarding";
		case Game::Character::State::Action::ActionState::UsingItem:
			return "UsingItem";
		default:
			break;
		}
	}
}