#include "Game/Sync/SyncLogicToTransformSystem.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Debug/DebugUtils.h"

#include <iostream>
// Logic -> Drawing
// 2D -> 3D
void SyncLogicToTransformSystem::Apply2DToTransform(ECS& ecs, float deltaTime)
{
	for (Entity e : ecs.view<Logic2DTransformComponent, TransformComponent>())
	{
		const auto& logic = ecs.get<Logic2DTransformComponent>(e);
		auto& transform = ecs.get<TransformComponent>(e);

		// 2D論理座標(x, y=0, z ) -> 3D描画座標 (x, y, z) (OpenGL)
		transform.position.x = logic.positionXZ.x;
		transform.position.y = 0.0f;// Fixed height (高さは固定)
		transform.position.z = logic.positionXZ.y;

		// scale ( xz -> xyz)
		// transform.scaleは論理スケールと一致する前提で固定。
		// 将来的に演出やモデル差異で変更が必要になった場合、RenderScaleComponentなどを導入する
		//transform.scale.x = logic.scale.x;
		//// ???????
		//transform.scale.y = 1.0f;
		//transform.scale.z = logic.scale.y;

		// rotation (logic.front から計算) (情報源はfrontなので、`logic.rotation`は避けている)
		transform.rotation.y = glm::degrees(logic.GetRotationYFromFrontVector());

		//// rotation (2D ローテーション　-> Y軸回りの回転)
		//float rotation = logic.rotation;
		//transform.rotation = glm::vec3(0.0f, rotation, 0.0f);

		// [Rotation Unit Policy]
		// - TransformComponent.rotation → degrees
		// - Logic2DTransformComponent.rotation → radians
		// - To sync from Transform to Logic: use glm::radians()
		// - To sync from Logic to Transform: use glm::degrees()


		//std::cout << "[SyncLogicToTransformSystem.cpp]: logical front vector " << logic.front.x << std::endl;
		//std::cout << "[SyncLogicToTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;

		//std::cout << "[SyncLogicToTransformSystem.cpp]: rotation " << logic.rotation <<"\n";

		// position log for debugging
		//DebugUtils::LogVector("SyncLogicToTransformSystem.cpp", transform.position);

		// DebugUtils::LogVector("SyncLogicToTransformSystem.cpp", logic.front);
	}

}

// 3D -> 3D
void SyncLogicToTransformSystem::Apply3DToTransform(ECS& ecs, float deltaTime)
{
	//for(Entity e: ecs.view<>)
}
