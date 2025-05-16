// �Q�[�����̒��ۑ���iConfirm, MoveUp�j
// Abstract operations in the game (Confirm, MoveUp)


#pragma once



enum class InputAction
{
	None = 0,

	// ���ʓI�ȑ���
	Confirm,
	Cancel,
	Pause,
	Quit,


	// �ړ�
	MoveForward,
	MoveBackward,
	MoveLeft,
	MoveRight,

	// ���_����C�W�����v�Ȃ�
	LookAround,
	Jump,

	// �X�L���E�U��
	CastSkill1,
	CastSkill2,
	CastSkill3,
	CastSkill4,

	// UI�p
	SelectUp,
	SelectDown,
	SelectLeft,
	SelectRight,
};