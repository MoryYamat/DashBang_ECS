#include "SpatialTransformUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Debug/DebugUtils.h"

glm::vec2 GameUtils::SpatialTransform::MouseTo2DLogicConverter(
	const glm::vec2& screenMousePos,
	const glm::mat4& viewMatrix,
	const glm::mat4& projectionMatrix,
	const glm::vec4& viewport,
	const glm::vec2& playerLogicPosXZ)
{
	// Screen coordinates �� NDC �� world ray (near/far)
	// �X�N���[�����W �� NDC �� ���[���h���C�inear/far�j (�}�E�X�|�C���^��RAY)
	glm::vec3 nearPoint = glm::unProject(glm::vec3(screenMousePos.x, viewport.w - screenMousePos.y, 0.0f),
		viewMatrix, projectionMatrix, viewport);

	glm::vec3 farPoint = glm::unProject(glm::vec3(screenMousePos.x, viewport.w - screenMousePos.y, 1.0f),
		viewMatrix, projectionMatrix, viewport);

	// �}�E�X���C �i�̃x�N�g��)
	glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

	// 0���Z�h�~
	if (rayDir.y == 0.0f) return glm::vec2(0.0f);
	// nearPoint.y + t * rayDir.y = (y=0)
	float t = -nearPoint.y / rayDir.y;
	// �n��(Y=0)�Ƃ̌����_ 
	glm::vec3 intersection = nearPoint + t * rayDir;

	// �L�����N�^�[����̑��Έʒu�x�N�g��(�_����Ԃł�)
	glm::vec2 mouseWorldXZ(intersection.x, intersection.z);
	// �������v�Z
	glm::vec2 direction = mouseWorldXZ - playerLogicPosXZ;

	//DebugUtils::LogPosition("SpatialTransformUtils.cpp", direction);

	// ���K������Ƃ悢
	return direction;
}

//
//logic.front = glm::normalize(
//	MouseTo2DLogicConverter(
//		input.screenMousePosition,
//		renderContext.viewMatrix,
//		renderContext.projectionMatrix,
//		renderContext.viewportSize,
//		logic.positionXZ
//	)
//);
//
//logic.rotation = glm::degrees(atan2(logic.front.x, logic.front.y));