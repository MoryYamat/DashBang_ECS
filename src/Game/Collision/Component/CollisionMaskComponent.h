// collision mask component

#pragma once 

#include "Game/Collision/Data/CollisionLayer.h"

#include "Game/ECS/Component/TeamComponent.h"

#include "Engine/ECS/Entity.h"

#include <cstdint>

namespace Game::Collision::Component
{
	// Collision Mask 自身の種類
	enum class Category : uint32_t
	{
		None = 0,
		CharacterBody	= 1u << 0,
		SkillHitbox		= 1u << 2,
		Tile			= 1u << 3,
		Sensor			= 1u << 4,
		All				= 0xFFFFFFFFu
	};

	// 
	enum class Relation : uint32_t
	{
		None	= 0,
		Self	= 1u << 0,
		Ally	= 1u << 1,
		Enemy	= 1u << 2,
		Neutral = 1u << 3,
		All		= 0xFFFFFFFFu
	};

	inline uint32_t bit(Relation r) { return static_cast<uint32_t>(r); }
	inline bool has(uint32_t mask, Relation r) { return (mask & bit(r)) != 0; }
	inline uint32_t bit(Category c) { return static_cast<uint32_t>(c); }

	struct CollisionMaskComponent
	{
		Category category = Category::None;// 自分は何者か
		uint32_t collideCategoryMask = 0; // 当たりたい相手のカテゴリ集合

		uint32_t relationMask = 0;	// 許容する相対関係の集合


		uint32_t flags = 0;		// Skill特性(Counteralbe/Unbockableなど)
		// Game::Collision::Data::Layer selfLayer = Game::Collision::Data::Layer::None;
		// uint32_t collidesWithMask = 0; // 自分が当たりたい対象のLayerの集合 ORしたマスク (bit mask)
	};


	inline bool shouldCollide(
		const CollisionMaskComponent& A,
		const CollisionMaskComponent& B,
		Relation rab,
		Relation rba
	)
	{
		const bool catOK =
			((A.collideCategoryMask & static_cast<uint32_t>(B.category)) != 0) &&
			((B.collideCategoryMask & static_cast<uint32_t>(A.category)) != 0);
		if (!catOK) return false;


		const bool relOK = has(A.relationMask, rab) && has(B.relationMask, rba);
		if (!relOK) return false;

		return true;
	};

	// 相対関係計算関数
	inline Game::Collision::Component::Relation computeRelation(Engine::ECS::Entity a, Engine::ECS::Entity b, 
		Game::ECS::Component::TeamComponent& ta, Game::ECS::Component::TeamComponent& tb)
	{
		if (a == b)
			return Relation::Self;

		if (ta.team == Game::ECS::Component::Team::Neutral || tb.team == Game::ECS::Component::Team::Neutral)
			return Relation::Neutral;

		if (tb.team == tb.team)
			return Relation::Ally;


		return Relation::Enemy;
	}

	namespace CollisionMaskPresets
	{
		inline CollisionMaskComponent Character()
		{
			CollisionMaskComponent c;
			c.category = Category::CharacterBody;
			c.collideCategoryMask = 
				bit(Category::CharacterBody) | 
				bit(Category::SkillHitbox) | 
				bit(Category::Sensor);

			c.relationMask = 
				bit(Relation::Ally) | 
				bit(Relation::Enemy) | 
				bit(Relation::Neutral);

			c.flags = 0;
			return c;
		}
	};
}