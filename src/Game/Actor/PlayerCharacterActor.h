// test player character actor

#pragma once

#include "Core/ECS/EntityManager.h"

#include "Graphics/Renderer/Shader.h"

class PlayerCharacter
{
public:
	PlayerCharacter(ECS& ecs, Shader* shader);

	~PlayerCharacter();

private:


};