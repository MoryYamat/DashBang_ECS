#include "CollisionResultStorage.h"


void CollisionResultStorage::Add(const CollisionContact& contact)
{
	mContacts.push_back(contact);
}

const std::vector<CollisionContact>& CollisionResultStorage::GetAll() const
{
	return mContacts;
}

void CollisionResultStorage::Clear()
{
	mContacts.clear();
	mTileCollisions.clear();
}


// EntityID Ç≈åüçı
std::vector<CollisionContact> CollisionResultStorage::GetContactsInvolving(Entity entity) const
{
	std::vector<CollisionContact> result;
	for (const auto& contact : mContacts)
	{
		if (contact.entityA == entity || contact.entityB == entity)
		{
			result.push_back(contact);
		}
	}
	return result;
}

void CollisionResultStorage::AddTileCollision(const std::vector<glm::ivec2>& indices)
{
	mTileCollisions.insert(mTileCollisions.end(), indices.begin(), indices.end());
}