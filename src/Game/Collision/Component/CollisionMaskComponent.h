// collision mask component

#pragma once 

#include "Game/Collision/Data/CollisionLayer.h"

#include "Game/Combat/Skill/Component/SkillOwnerComponent.hpp"
#include "Game/ECS/Component/TeamComponent.h"

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

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

	inline bool shouldCollideWithCat(
		CollisionMaskComponent& A,
		CollisionMaskComponent& B
	)
	{
		const bool catOK =
			((A.collideCategoryMask & static_cast<uint32_t>(B.category)) != 0) &&
			((B.collideCategoryMask & static_cast<uint32_t>(A.category)) != 0);
		if (!catOK) return false;

		return true;
	}

	inline bool shouldCollideWithRel(
		const CollisionMaskComponent& A,
		const CollisionMaskComponent& B,
		Relation rab,
		Relation rba
	)
	{

		const bool relOK = has(A.relationMask, rab) && has(B.relationMask, rba);
		if (!relOK) return false;

		return true;
	};

	inline Game::ECS::Component::Team resolveTeam(Engine::ECS::EntityMgr& ecs, eNsECS::Entity e)
	{
		using namespace Game::Combat::Skill::Component;
		using namespace Game::ECS::Component;
		if (ecs.hasComponent<TeamComponent>(e))
		{
			return ecs.get<TeamComponent>(e).team;
		}
		if (ecs.hasComponent<SkillOwnerComponent>(e))
		{
			auto owner = ecs.get<SkillOwnerComponent>(e).caster;
			return resolveTeam(ecs, owner);
		}

		return Team::Neutral;
	};

	// 相対関係計算関数
	inline Game::Collision::Component::Relation computeRelation(
		Engine::ECS::EntityMgr& ecs,
		Engine::ECS::Entity a, Engine::ECS::Entity b)
	{
		using namespace Game::ECS::Component;

		if (a.id == b.id)
			return Relation::Self;

		// TODO: 以下のFIXMEは問題にならないかもしれない．キチンとシナリオを追って完全性を証明する必要あり
		// FIXME: 現在，caster == a : Ally になってしまう．
		// これは，caster == a : ならば，Selfにならなければいけない．
		// つまり， ally: if(a is not skillEntity or b is not skillEntity) and if(ta == tb)
		Team ta = resolveTeam(ecs, a);
		Team tb = resolveTeam(ecs, b);

		if (ta == Game::ECS::Component::Team::Neutral || tb == Game::ECS::Component::Team::Neutral)
			return Relation::Neutral;

		if (ta == tb)
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