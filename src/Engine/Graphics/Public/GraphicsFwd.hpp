#pragma once

namespace Engine::Graphics
{
	class Shader;
	struct RenderContext;
}

// 以下廃止予定
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
}
	