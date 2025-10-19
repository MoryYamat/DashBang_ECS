// GPU Buffer Utils
#pragma once



#include "Engine/Graphics/Model/ModelData.h"


namespace Engine::Graphics::Render
{
	namespace GPUBufferUtils
	{
		// 
		Engine::Graphics::Model::ModelGPU createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modeldata);
	}
}
