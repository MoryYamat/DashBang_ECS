// GPU Buffer Utils
#pragma once



#include "Engine/Graphics/Model/ModelData.h"

#include "Common/EngineNamespaceDecl.h"

namespace Engine::Graphics::Render
{
	namespace GPUBufferUtils
	{
		// 
		eNsGfxModel::ModelGPU createMeshGPUBuffers(const eNsGfxModel::ModelData& modeldata);
	}
}
