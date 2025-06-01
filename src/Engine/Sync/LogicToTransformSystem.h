// Linking logic coordinates with drawing coordinates

#pragma once

#include "Engine/ECS/EntityManager.h"

#include "Common/EngineNamespaceDecl.h"

// Logic -> Drawing
// ロジック　-> 描画
namespace Engine::Sync::LogicToTransformSystem
{
	// 2D -> 3D
	void Apply2DToTransform(eNsECS::EntityMgr& ecs, float deltaTime);

	// 3D -> 3D
	void Apply3DToTransform(eNsECS::EntityMgr& ecs, float deltaTime);


	// 2D -> 2D
	//void Apply2D
};