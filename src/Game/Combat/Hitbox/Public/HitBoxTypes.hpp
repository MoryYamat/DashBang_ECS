#pragma once

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"
#include "Engine/Component/Private/Logic2D/ColliderType.hpp"

#include "Game/Combat/Skill/Public/SkillFwd.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"
#include "Game/Combat/HitBox/Public/DTO.hpp"

#include <vector>
#include <unordered_map>

#include <GLM/glm.hpp>

namespace Game::Combat::HitBox
{
	struct CanonicalHitBoxData
	{
		HitBoxID id;								// 定義順に0～採番
		Engine::Component::ShapeKind shape = Engine::Component::ShapeKind::None;			// 将来 enmu に正規化
		float radius = 0.f;			// circle用
		float angle = 0.f;			// Sector 用 (degree or rad)
		float length = 0.f;			// Rect/Sector用
		glm::vec2 offset = glm::vec2(0.f);		// owner基準
	};

	struct HitBoxCatalog
	{
		std::vector<CanonicalHitBoxData> hitBoxes;


		std::unordered_map<std::string, HitBoxID> nameToId;
		HitBoxID FindByName(std::string_view s) const;
	};


	// Buffers
	struct HitBoxSpawnRequest
	{
		Engine::ECS::Core::Entity owner;
		HitBoxID hitbox;
		Game::Combat::Skill::SkillID skill;		// どのスキル由来か
		Engine::FSM::Core::StateID state;		// どのフェーズで発生したか
		float triggerTime;						// Skill 内 の timeoffset
	};


	// pubilc
	struct HitBoxSpawnRequestBuffer
	{
		std::vector<HitBoxSpawnRequest> reqs;
		void clear() { reqs.clear(); }
	};


	//// HITBOX & SKill Mapping
	//struct SkillHitBoxMapping
	//{
	//	 std::unordered_map<Game::Combat::Skill::SkillID, HitBoxID> skill2hitbox;

	//	HitBoxID Resolve(Game::Combat::Skill::SkillID s) const
	//	{
	//		auto it = skill2hitbox.find(s);
	//		if (it == skill2hitbox.end()) return kInvalidHitBoxID;
	//		return it->second;
	//	}

	//	void Set(Game::Combat::Skill::SkillID s, HitBoxID h)
	//	{
	//		skill2hitbox[s] = h;
	//	}
	//};

	struct HitBoxComponent
	{
		HitBoxID def;
		Engine::ECS::Core::Entity owner;
		float lifetime;
	};

	struct HitBoxSpawnSystem
	{
		Engine::WorldSystem::Core::WorldCtx& ctx;

		void Update(const float dt);
	};
}