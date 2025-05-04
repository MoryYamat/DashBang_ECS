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

		// 2D�_�����W(x, y=0, z ) -> 3D�`����W (x, y, z) (OpenGL)
		transform.position.x = logic.positionXZ.x;
		transform.position.y = 0.0f;// Fixed height (�����͌Œ�)
		transform.position.z = logic.positionXZ.y;

		// scale ( xz -> xyz)
		// transform.scale�͘_���X�P�[���ƈ�v����O��ŌŒ�B
		// �����I�ɉ��o�⃂�f�����قŕύX���K�v�ɂȂ����ꍇ�ARenderScaleComponent�Ȃǂ𓱓�����
		//transform.scale.x = logic.scale.x;
		//// ???????
		//transform.scale.y = 1.0f;
		//transform.scale.z = logic.scale.y;

		// rotation (2D ���[�e�[�V�����@-> Y�����̉�])
		float rotation = logic.rotation;
		transform.rotation = glm::vec3(0.0f, rotation, 0.0f);


		//std::cout << "[SyncLogicToTransformSystem.cpp]: logical front vector " << logic.front.x << std::endl;
		//std::cout << "[SyncLogicToTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;

		//std::cout << "[SyncLogicToTransformSystem.cpp]: rotation " << logic.rotation <<"\n";

		// position log for debugging
		//DebugUtils::LogPosition("SyncLogicToTransformSystem.cpp", transform.position);

		DebugUtils::LogPosition("SyncLogicToTransformSystem.cpp", logic.front);
	}

}

// 3D -> 3D
void SyncLogicToTransformSystem::Apply3DToTransform(ECS& ecs, float deltaTime)
{
	//for(Entity e: ecs.view<>)
}
