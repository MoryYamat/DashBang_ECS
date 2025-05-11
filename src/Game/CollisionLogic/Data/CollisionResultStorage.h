//

#pragma once

#include "Core/ECS/EntityManager.h"

#include <glm/glm.hpp>

#include <vector>

struct CollisionContact
{
	Entity entityA;// �Փˑ�A
	Entity entityB;// �Փˑ�B
	glm::vec2 contactNormal;// �Փ˖ʖ@������
	float penetrationDepth;// �߂荞�ݗ�

	CollisionContact(Entity a, Entity b, const glm::vec2& normal = glm::vec2(0.0f), float penetration = 0.0f)
		: entityA(a), entityB(b), contactNormal(normal), penetrationDepth(penetration) {
	}
};

class CollisionResultStorage
{
public:

	// 
	void Add(const CollisionContact& contact);

	// 
	const std::vector<CollisionContact>& GetAll() const;

	// 1�t���[�����Ƃɏ�����
	// Initialize every frame
	void Clear();

	// ����G���e�B�e�B�Ɋւ���Փ˂������擾
	std::vector<CollisionContact> GetContactsInvolving(Entity entity) const;

	// �ՓˑΏۏ��̊i�[
	void AddTileCollision(const std::vector<glm::ivec2>& indices);

	const std::vector<glm::ivec2>& GetTileCollisions() const
	{
		return mTileCollisions;
	};

private:
	std::vector<CollisionContact> mContacts;

	std::vector<glm::ivec2> mTileCollisions;
};