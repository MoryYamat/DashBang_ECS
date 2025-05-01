#include "CameraFollowSystem.h"

#include "Core/ECS/Component/TransformComponent.h"
#include "Core/ECS/Component/FollowCameraComponent.h"

void CameraFollowSystem::Update(ECS& ecs, float deltaTime)
{
	for (Entity e : ecs.view<FollowCameraComponent, TransformComponent>())
	{
		auto& follow = ecs.get<FollowCameraComponent>(e);
		auto& camTransform = ecs.get<TransformComponent>(e);

		if (ecs.hasComponent<TransformComponent>(Entity{ follow.targetEntity })) continue;

		const auto& targetTransform = ecs.get<TransformComponent>(Entity{ follow.targetEntity });

		// �Ǐ](�I�t�Z�b�g)
		camTransform.position = targetTransform.position + follow.offset;

		// ������Ǐ]�Ώۂ�
		glm::vec3 direction = glm::normalize(targetTransform.position - camTransform.position);

	}
}