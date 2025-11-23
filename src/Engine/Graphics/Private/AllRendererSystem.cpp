#include "Engine/Graphics/Public/GraphicsApi.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"


#include "Engine/Graphics/Public/Types.hpp"

#include <iostream>

namespace Engine::Graphics
{
	void UpdateRendererAll(Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::Graphics::Shader& shader, float aspect, RenderContext& context)
	{
		RendererPipeline pipeline;
		BuildRendererPipeline(ctx, pipeline);
		
		// animation 
		Engine::Graphics::Animation::AnimationSystem(ctx);

		// render
		RenderSystem(ctx, shader, pipeline, aspect, context);
	}
}