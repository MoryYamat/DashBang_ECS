#pragma once

#include "GraphicsFwd.hpp"
#include "Types.hpp"
#include "Engine/WorldSystem/Public/WorldFwd.hpp"

#include <span>

namespace Engine::Graphics
{
	void RenderSystem(Engine::WorldSystem::Core::WorldCtx& ctx, Engine::Graphics::Shader& shader, std::span<RendererPipelineEntry> ents,
		float aspect, RenderContext& context);

	void BuildRendererPipeline(Engine::WorldSystem::Core::WorldCtx& ctx, RendererPipeline& out);
	void UpdateRendererAll(Engine::WorldSystem::Core::WorldCtx& ctx, 
		Engine::Graphics::Shader& shader, float aspect, RenderContext& context);

	Engine::Graphics::Model::ModelGPU createMeshGPUBuffers(const Engine::Graphics::Model::ModelData& modelData);
}

namespace Engine::Graphics::Animation
{
	void AnimationSystem(Engine::WorldSystem::Core::WorldCtx& ctx);
}