// キャラクターの行動に関する状態を管理するための状態群を定義

#pragma once


namespace Game::Character::State
{
    // 行動状態定義
	enum class CharacterBehaviorState
	{
		Idle,		// 待機
		Moving,	// 入力によって移動中
		Action,	// 移動以外の行動
        Dead,		// 死亡
	};


    // 行動制限状態定義
    enum class CharacterCrowdControlState
    {
        None,
        Stunned,
        Knockbacked,
        Frozen,
    };


    // オプション：状態名出力
    inline const char* ToString(CharacterBehaviorState state)
    {
        switch (state)
        {
        case CharacterBehaviorState::Idle:        return "Idle";
        case CharacterBehaviorState::Moving:      return "Moving";
        case CharacterBehaviorState::Action:   return "Action";
        case CharacterBehaviorState::Dead:        return "Dead";
        default:                          return "Unknown";
        }
    }

    // オプション：状態名出力
    inline const char* ToString(CharacterCrowdControlState state)
    {
        switch (state)
        {
            case CharacterCrowdControlState::None:       return "None";
            case CharacterCrowdControlState::Stunned: return "Stunned";
            case CharacterCrowdControlState::Knockbacked: return "Knockbacked";
            case CharacterCrowdControlState::Frozen:      return "Frozen";
        }
    }
}