//

#pragma once

#include "Engine/ECS/EntityManager.h"

#include <glm/glm.hpp>

#include <vector>

#include "Common/EngineNamespaceDecl.h"

#include "Common/GameNamespaceDecl.h"

#include <glm/glm.hpp>
#include <cstdint>

namespace Game::Collision::Data
{
	enum class ContactChannel : uint8_t
	{
		Solid,
		Hitbox
	};// 物理 or ヒット

	enum class ContactPhase : uint8_t
	{
		Begin,
		Stay,
		End
	};//

	struct ContactPayload
	{
		glm::vec2 normal{ 0,0 };
		float depth = 0.0f;

		int skillId = -1;
		int hitboxId = -1;
		glm::vec2 impactDir{ 0,0 };
		float impactMag = 0.0f;
	};

	struct Contact
	{
		eNsECS::Entity a;
		eNsECS::Entity b;
		ContactChannel channel;
		ContactPhase phase;
		ContactPayload payload;
		uint64_t key; // (a,b,channel) ハッシュ
	};

	// 衝突の詳細情報(法線方向など)
	struct ContactInfo
	{
		glm::vec2 contactNormal;// 衝突面法線方向
		float penetrationDepth;// めり込み量
	};

	// 衝突結果
	struct CollisionResult
	{
		eNsECS::Entity entityA;// 衝突体A
		eNsECS::Entity entityB;// 衝突体B
		ContactInfo contact;// 衝突の詳細情報(法線方向など)
	};

	struct ContactBus
	{
		std::vector<Contact> begin, stay, end;
		void clear() { begin.clear(); stay.clear(); end.clear(); }
		void push(const Contact& c)
		{
			if (c.phase == ContactPhase::Begin)
			{
				begin.push_back(c);
			}
			else if (c.phase == ContactPhase::Stay)
			{
				stay.push_back(c);
			}
			else
			{
				end.push_back(c);
			}
		}
	};

	// Bufferデータ(毎フレーム更新)
	struct CollisionResultBuffer
	{
		std::vector<CollisionResult> results;
		void clear() { results.clear(); }
		void add(const CollisionResult& result) { results.push_back(result); }
	};

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