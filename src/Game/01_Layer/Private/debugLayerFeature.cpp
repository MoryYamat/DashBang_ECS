#include "Game/01_Layer/Public/LayerFeature.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Game/00_Feature/Private/Debug/DebugFeatureAPI.hpp"

// 任意のテストコードを流す用のレイヤーです
// まだ未使用ですが、必要時に適宜整備します
namespace Game::Layer::Debug
{
	//void DebugLayerFeature::Update(Engine::ECS::EntityMgr& ecs)
	//{

	//}
	void DebugLayerFeature::InitDebugSystem(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		Game::Feature::Debug::DebugFeature::InitDebugDrawSystem(ctx);
	}

	void DebugLayerFeature::Update(Engine::WorldSystem::Core::WorldCtx& ctx)
	{
		
	}

	void DebugLayerFeature::UpdateDebugRender(Engine::WorldSystem::Core::WorldCtx& ctx,
		Engine::Graphics::RenderContext& renderCtx_,
		Engine::Debug::DebugLineRenderer& debugLineRenderer_)
	{ 

		Game::Feature::Debug::DebugFeature::UpdateDebugDrawSystem(ctx, renderCtx_, debugLineRenderer_);
	}
}