// �R���|�[�l���g�̏������t�F�[�Y���w�肷�邽�߂̃e���v���[�g�Q
// Templates for specifying the initialization phase of a component

#pragma once


enum class InitPhase
{
	Immediate,
	Deferred
};

template<typename Component>
constexpr InitPhase GetInitPhase()
{
	return InitPhase::Immediate;
}