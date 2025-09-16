#pragma once

#include "Engine/ECS/Entity.h"

#include <cstdint>
#include <unordered_set>

namespace Game::Combat::Skill::Component
{
	struct HitboxHitMemoComponent
	{
		std::unordered_set<uint32_t> hitTargets;// EntityIDだけを格納

		inline bool alreadyHit(Engine::ECS::Entity target) const
		{
			return hitTargets.find(target.id) != hitTargets.end();
		};

		void markHit(Engine::ECS::Entity target)
		{
			hitTargets.insert(target.id);
		}
	};

}