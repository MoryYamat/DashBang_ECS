#pragma once

#include "Engine/Graphics/Model/ModelData.h"

#include "cgltf.h"


namespace Engine::Graphics::Model
{
	class CgltfImporter
	{
	public:
		static Engine::Graphics::Model::ModelData Import(const std::string& path);
	};
}