// �e�L�����N�^( Player / NPC) ���ǂ̃X�L���������Ă��邩���`����R���|�[�l���g
// �܂菊�����X�g
#pragma once

#include <unordered_set>

// �ێ�������S�X�L���̃��X�g
struct SkillSetComponent
{
	std::unordered_set<int> ownedSkillIds;
};