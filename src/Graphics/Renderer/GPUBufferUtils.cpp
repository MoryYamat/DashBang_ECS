#include "GPUBufferUtils.h"

#include <iostream>

ModelGPU GPUBufferUtils::createMeshGPUBuffers(const ModelData& modelData)
{
	ModelGPU modelGPU;


	for (const auto& meshData : modelData.meshes)
	{
		MeshGPU meshGPU;

		// generate VAO, VBO, EBO
		glGenVertexArrays(1, &meshGPU.vao);
		glGenBuffers(1, &meshGPU.vbo);
		glGenBuffers(1, &meshGPU.ebo);

		glBindVertexArray(meshGPU.vao);

		// vbo
		glBindBuffer(GL_ARRAY_BUFFER, meshGPU.vbo);
		glBufferData(GL_ARRAY_BUFFER, meshData.vertices.size() * sizeof(VertexData), meshData.vertices.data(), GL_STATIC_DRAW);

		// ebo
		if (meshData.hasIndices && !meshData.indices.empty())
		{
			// インデックス数
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshGPU.ebo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, meshData.indices.size() * sizeof(unsigned int), meshData.indices.data(), GL_STATIC_DRAW);
			meshGPU.indexCount = static_cast<unsigned int>(meshData.indices.size());
		}
		else
		{
			// 頂点数
			meshGPU.indexCount = static_cast<unsigned int>(meshData.vertices.size());
		}

		// vao
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (void*)offsetof(VertexData, position));

		glBindVertexArray(0);

		modelGPU.meshesGPU.push_back(meshGPU);

	}

	std::cout << "The modelData was successfully converted to GPUmodelData." << std::endl;

	return modelGPU;
}