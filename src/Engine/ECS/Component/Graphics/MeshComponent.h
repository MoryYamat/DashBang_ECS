// mesh data component

#include <GLAD/glad.h>

#include <GLM/glm.hpp>

#include "Engine/Graphics/Model/ModelData.h"

#include <iostream>



namespace Engine::ECS::Component::Graphics
{
	struct MeshComponent
	{
		// Mesh Resources
		eNsGfxModel::ModelData modelData;
		eNsGfxModel::ModelGPU modelGPU;

		MeshComponent() = default;
		~MeshComponent() = default;

		MeshComponent(const MeshComponent&) = delete;
		MeshComponent& operator=(const MeshComponent&) = delete;

		MeshComponent(MeshComponent&&) noexcept = default;
		MeshComponent& operator=(MeshComponent&&) noexcept = default;

		// 追加：ムーブ専用の受け取り
		MeshComponent(eNsGfxModel::ModelData&& md, eNsGfxModel::ModelGPU&& mg)
			: modelData(std::move(md)), modelGPU(std::move(mg)) {
		}
	};

	// memo 
	// SIMD single instruction multiple data (CPU and memory/cache)
	//
}

