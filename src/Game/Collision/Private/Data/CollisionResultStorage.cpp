#include "CollisionResultStorage.h"


void Game::Collision::Data::CollisionResultStorage::Add(const CollisionContact& contact)
{
	mContacts.push_back(contact);
}

const std::vector<Game::Collision::Data::CollisionContact>& Game::Collision::Data::CollisionResultStorage::GetAll() const
{
	return mContacts;
}

void Game::Collision::Data::CollisionResultStorage::Clear()
{
	mContacts.clear();
	mTileCollisions.clear();
}


// EntityID で検索
std::vector<Game::Collision::Data::CollisionContact> Game::Collision::Data::CollisionResultStorage::GetContactsInvolving(Engine::ECS::Entity entity) const
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

void Game::Collision::Data::CollisionResultStorage::AddTileCollision(const std::vector<glm::ivec2>& indices)
{
	mTileCollisions.insert(mTileCollisions.end(), indices.begin(), indices.end());
}

