//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include <glm/glm.hpp>

#include <vector>

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

namespace Game::Collision::Data
{
	struct CollisionContact
	{
		eNsECS::Entity entityA;// 衝突体A
		eNsECS::Entity entityB;// 衝突体B
		glm::vec2 contactNormal;// 衝突面法線方向
		float penetrationDepth;// めり込み量

		CollisionContact(eNsECS::Entity a, eNsECS::Entity b, const glm::vec2& normal = glm::vec2(0.0f), float penetration = 0.0f)
			: entityA(a), entityB(b), contactNormal(normal), penetrationDepth(penetration) {
		}
	};

	// 1フレームのコリジョン情報バッファ
	class CollisionResultStorage
	{
	public:

		// 
		void Add(const CollisionContact& contact);

		// 
		const std::vector<CollisionContact>& GetAll() const;

		// 1フレームごとに初期化
		// Initialize every frame
		void Clear();

		// 特定エンティティに関する衝突だけを取得
		std::vector<CollisionContact> GetContactsInvolving(eNsECS::Entity entity) const;

		// 衝突対象情報の格納
		void AddTileCollision(const std::vector<glm::ivec2>& indices);

		const std::vector<glm::ivec2>& GetTileCollisions() const
		{
			return mTileCollisions;
		};

	private:
		std::vector<CollisionContact> mContacts;

		std::vector<glm::ivec2> mTileCollisions;


		// unordered_multimap<Entity, CollisionContact> を併用して検索最適化

		// FrameScopedStorage<CollisionResultStorage> などでライフサイクル管理の明示化
	};


}