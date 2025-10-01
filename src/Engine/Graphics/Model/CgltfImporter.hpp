#pragma once

#include "Engine/Graphics/Model/ModelData.h"

#include "cgltf.h"

#include "stb_image.h"

#include <glad/glad.h>

#include <string>

namespace Engine::Graphics::Model
{
	class CgltfImporter
	{
	public:
		static Engine::Graphics::Model::ModelData Import(const std::string& path);


	private:
		static bool GetImageBytes_FromBufferView(
			const cgltf_image* img,
			const cgltf_data* g,
			const unsigned char*& bytes,
			size_t& size
		);

		static GLuint CreateGLTexture2D_FromMemory_sRGB(
			const unsigned char* bytes, size_t size
		);
	};
}