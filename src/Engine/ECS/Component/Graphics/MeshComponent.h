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

		void Destroy()
		{
			for (auto& mesh : modelGPU.meshesGPU)
			{
				if (mesh.ebo != 0) glDeleteBuffers(1, &mesh.ebo);
				if (mesh.vbo != 0) glDeleteBuffers(1, &mesh.vbo);
				if (mesh.vao != 0) glDeleteVertexArrays(1, &mesh.vao);


				mesh.vao = mesh.vbo = mesh.ebo = 0;
			}

			std::cout << "\n[MeshComponent.h]: destroyer called." << std::endl;
		}
	};

	// memo 
	// SIMD single instruction multiple data (CPU and memory/cache)
	//
}

