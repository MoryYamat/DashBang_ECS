#pragma once

#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include "Engine/ECS/Public/Entity.hpp"
#include "Engine/FSM/Public/FSMFwd.hpp"

#include <cstdint>

namespace Game::Combat::Skill
{
	enum class LogicCommandKind : std::uint8_t
	{
		SpawnHitBox,
		PlayAnim,
	};

	struct LogicCommand
	{
		LogicCommandKind kind;
		Engine::ECS::Core::Entity owner = Engine::ECS::Core::INVALID;
		SkillID skill;
		Engine::FSM::Core::StateID state;
		float effectTime;					// 発火したタイミング?
	};

	// フレームごとの一時リソース
	struct SkillLogicCommandBuffer
	{
		std::vector<LogicCommand> cmds;
		void clear() { cmds.clear(); }
	};
}