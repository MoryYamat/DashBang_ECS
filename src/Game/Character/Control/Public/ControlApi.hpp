#pragma once

#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include "Game/Combat/Skill/Public/SkillFwd.hpp"

#include <array>
#include <cstddef>
#include <cstdint>

namespace Game::Character::Control
{
	void UpdatePlayerSkillIntent(Engine::WorldSystem::Core::WorldCtx& ctx);

}