// Linking logic coordinates with drawing coordinates

#pragma once

#include "Core/ECS/EntityManager.h"

// Logic -> Drawing
// ロジック　-> 描画
namespace SyncLogicToTransformSystem
{
	// 2D -> 3D
	void Apply2DToTransform(ECS& ecs, float deltaTime);

	// 3D -> 3D
	void Apply3DToTransform(ECS& ecs, float deltaTime);


	// 2D -> 2D
	//void Apply2D
};