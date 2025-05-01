#pragma once

#include "DataTypes/ModelData.h"

#include <string>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

namespace AssimpImporter
{
	ModelData Import(const std::string& path);

	void processNode(aiNode* node, const aiScene* scene, ModelData& modelData);

	MeshData processMesh(aiMesh* mesh, const aiScene* scene, ModelData& modelData);

}