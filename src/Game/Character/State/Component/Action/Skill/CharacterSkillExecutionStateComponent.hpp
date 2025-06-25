#pragma once


namespace Game::Character::State::Action
{
	// キャラクターのスキル実行状態
	enum class CharacterSkillExecutionPhase
	{
		None, // スキル実行状態なし
		Casting, // スキルキャスト中
		Active, // スキル実行中
		Recovery, // 硬直
		Completed, // スキル実行完了
		Interrupted // スキル実行中断
	};

	// キャラクターのスキル実行状態を表すコンポーネント
	struct CharacterSkillExecutionStateComponent
	{
		CharacterSkillExecutionPhase currentPhase = CharacterSkillExecutionPhase::None; // スキル実行状態
		CharacterSkillExecutionPhase previousPhase = CharacterSkillExecutionPhase::None; // 前のスキル実行状態
		float elapsedTime = 0.0f; // 経過時間
	};
}