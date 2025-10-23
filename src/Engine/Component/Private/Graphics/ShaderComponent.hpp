#pragma once

#include "Engine/Graphics/Renderer/Shader.h"

namespace Engine::Component
{
	struct ShaderComponent
	{
		Engine::Graphics::Render::Shader* shader;
	};
}