// ゲーム内の抽象操作（Confirm, MoveUp）
// Abstract operations in the game (Confirm, MoveUp)


#pragma once



enum class InputAction
{
	None = 0,

	// 共通的な操作
	Confirm,
	Cancel,
	Pause,
	Quit,


	// 移動
	MoveForward,
	MoveBackward,
	MoveLeft,
	MoveRight,

	// 視点操作，ジャンプなど
	LookAround,
	Jump,

	// スキル・攻撃
	CastSkill1,
	CastSkill2,
	CastSkill3,
	CastSkill4,

	// UI用
	SelectUp,
	SelectDown,
	SelectLeft,
	SelectRight,
};