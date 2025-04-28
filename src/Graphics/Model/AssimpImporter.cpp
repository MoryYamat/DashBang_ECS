#include "AssimpImporter.h"

#include <iostream>


ModelData AssimpImporter::Import(const std::string& path)
{
	ModelData data;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path,
		aiProcess_Triangulate |
		aiProcess_GenSmoothNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cerr << "[AssimpImporter.cpp(Import)]: Error Assimp: " << importer.GetErrorString() << std::endl;
		return data;
	}
	else
	{
		std::cout << "[AssimpImporter.cpp(Import)]: Load Completed: [[\"" << path << "\"]]" << std::endl;
	}

	processNode(scene->mRootNode, scene, data);

	return data;
}


void AssimpImporter::processNode(aiNode* node, const aiScene* scene, ModelData& modelData)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		modelData.meshes.push_back(processMesh(mesh, scene, modelData));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, modelData);
	}
}

MeshData AssimpImporter::processMesh(aiMesh* mesh, const aiScene* scene, ModelData& modelData)
{
	//static int x = 1;
	//std::cout << "[assimpimporter.cpp]: mesh num:" << x << std::endl;
	//x++;


	MeshData meshData;
	VertexData vertexData;

	glm::vec3 vector;

	//�@���_�����f�[�^
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertexData.position = vector;

		if (mesh->HasNormals())
		{

		}

		if (mesh->mTextureCoords[0])
		{

		}
		else
		{

		}
		meshData.vertices.push_back(vertexData);
	}

	// �C���f�b�N�X�f�[�^
	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			meshData.indices.push_back(face.mIndices[j]);
			//std::cout << "inputed indices: " << face.mIndices[j] << std::endl;
		}
	}

	// indices�������True, �Ȃ����False;
	meshData.hasIndices = !meshData.indices.empty();

	return meshData;
}