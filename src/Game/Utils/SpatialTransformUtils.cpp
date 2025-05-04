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
	// Screen coordinates → NDC → world ray (near/far)
	// スクリーン座標 → NDC → ワールドレイ（near/far） (マウスポインタのRAY)
	glm::vec3 nearPoint = glm::unProject(glm::vec3(screenMousePos.x, viewport.w - screenMousePos.y, 0.0f),
		viewMatrix, projectionMatrix, viewport);

	glm::vec3 farPoint = glm::unProject(glm::vec3(screenMousePos.x, viewport.w - screenMousePos.y, 1.0f),
		viewMatrix, projectionMatrix, viewport);

	// マウスレイ （のベクトル)
	glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

	// 0除算防止
	if (rayDir.y == 0.0f) return glm::vec2(0.0f);
	// nearPoint.y + t * rayDir.y = (y=0)
	float t = -nearPoint.y / rayDir.y;
	// 地面(Y=0)との交差点 
	glm::vec3 intersection = nearPoint + t * rayDir;

	// キャラクターからの相対位置ベクトル(論理空間での)
	glm::vec2 mouseWorldXZ(intersection.x, intersection.z);
	// 方向を計算
	glm::vec2 direction = mouseWorldXZ - playerLogicPosXZ;

	//DebugUtils::LogPosition("SpatialTransformUtils.cpp", direction);

	// 正規化するとよい
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