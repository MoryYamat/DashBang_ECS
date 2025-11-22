#include "Game/Character/Private/FSM/Public/MovementAxisApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Component/Private/Logic2D/Velocity2DComponent.hpp"
#include "Game/Character/Private/Stats/Public/StatsComponent.hpp"
#include "Game/Character/Private/Control/Public/IntentComponent.hpp"

#include <iostream>
#include <glm/glm.hpp>

namespace Game::Character::FSM::Movement
{
	using namespace Engine::WorldSystem::Core;
	using namespace Engine::Component;

	// intentから移動方向速度ベクトルを計算
	void SetMovementVelComp(const MovementPipelineEntry& entry, Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& vel = *entry.vel;

		glm::vec2 dir{ 0.0f };
		if (const auto* intent = ctx.ww.TryGetWithWarnOnce<Game::Character::Control::MovingIntentComponent>(entry.e))
		{
			dir = intent->direction;
		}

		float speed = 1.0f;
		if (const auto* stats = ctx.ww.TryGetWithWarnOnce<Game::Character::Stats::CharacterStatsComponent>(entry.e))
		{
			speed = stats->moveSpeed;
		}

		// std::cout << "[speed] " << speed << "\n";
		vel.velocity = dir * speed;
	}

	// 速度を0に設定
	void SetZeroVel(const MovementPipelineEntry& entry, Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		auto& vel = ctx.ww.Get<Velocity2DComponent>(entry.e);
		vel.velocity = glm::vec2(0.0f);
	}
}