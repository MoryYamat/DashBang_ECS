#pragma once


#include <GLAD/glad.h>

namespace Engine::ECS::Component::Graphics
{
	struct ShaderComponent
	{
		eNsGfxRender::Shader* shader;
	};
}

