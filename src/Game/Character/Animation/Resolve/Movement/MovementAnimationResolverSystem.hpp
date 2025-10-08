#pragma once

#include "Engine/ECS/EntityManager.h"

#include "MovementAnimDecisionComponent.hpp"

#include <glm/glm.hpp>

namespace Game::Character::Animation::Resolve::Movement
{
	// 再生したいアニメーションの候補を決める
	struct MovementAnimationResolverSystem
	{
		static void Update(Engine::ECS::EntityMgr& ecs);

	private:
		// 最適化-> 参照でもよいのでは？
		static Game::Character::Animation::Resolve::Movement::MoveAnimType 
			calcRelativeMovementDir(const glm::vec2 facingDir, const glm::vec2 movingDir,
				const Game::Character::Animation::Resolve::Movement::MoveAnimType prev);
	};
}