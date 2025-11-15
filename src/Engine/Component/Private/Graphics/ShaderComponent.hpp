#pragma once

#include "Engine/Graphics/Private/Renderer/Shader.h"

namespace Engine::Component
{
	struct ShaderComponent
	{
		Engine::Graphics::Render::Shader* shader;
	};
}