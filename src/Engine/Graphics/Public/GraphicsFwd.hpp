#pragma once


namespace Engine::Graphics
{
	namespace Animation
	{
		class Animator;
		struct TRS;
	}

	namespace Model
	{
		struct TextureData;
		struct MaterialData;
		struct VertexData;
		struct Bone;
		struct Skeleton;
		enum class ChannelType;
		struct Channel;
		struct AnimationClip;
		struct MeshData;
		struct ModelData;
		struct MeshGPU;
		struct ModelGPU;
	}

	namespace Render
	{
		struct RenderContext;
		class Shader;
	}
}
	