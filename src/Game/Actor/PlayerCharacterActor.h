// test player character actor
// Class that creates an entity

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