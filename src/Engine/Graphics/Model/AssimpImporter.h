#pragma once

#include "Engine/Graphics/Model/ModelData.h"

#include <string>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Common/EngineNamespaceDecl.h"

namespace Engine::Graphics::Model
{
	namespace AssimpImporter
	{
		eNsGfxModel::ModelData Import(const std::string& path);

		void processNode(aiNode* node, const aiScene* scene, eNsGfxModel::ModelData& modelData);

		eNsGfxModel::MeshData processMesh(aiMesh* mesh, const aiScene* scene, eNsGfxModel::ModelData& modelData);

	}
}

