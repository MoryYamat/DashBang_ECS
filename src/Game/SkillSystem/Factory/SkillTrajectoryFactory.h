//


#pragma once

#include "Game/SkillSystem/MasterData/SkillDefinition.h"
#include "Core/ECS/Component/Transform2DComponent.h"

#include <functional>

class SkillTrajectoryFactory
{
public:
	// SkillDefinition + Origin Transform ‚©‚ç‹OÕŠÖ”‚ğ¶¬
	static std::function<Transform2DComponent(float)> Create(
		const SkillDefinition& def,
		const Transform2DComponent& origin);
};