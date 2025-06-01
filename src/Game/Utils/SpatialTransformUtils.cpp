#include "SpatialTransformUtils.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Engine/Debug/DebugUtils.h"

glm::vec2 Game::Utils::GetLogicDirectionFromCursorToTarget(
	const glm::vec2& screenMousePos, 
	const eNsGfxRender::RenderContext& renderContext,
	const glm::vec2& playerLogicPosXZ // Character position in logical coordinates
)
{
	glm::vec2 mouseWorldXZ = ProjectScreenToLogicXZPlane(screenMousePos, renderContext);

	// キャラクターからの相対位置ベクトル(論理空間での)方向を計算
	glm::vec2 direction = mouseWorldXZ - playerLogicPosXZ;

	//DebugUtils::LogVector("SpatialTransformUtils.cpp(mouseToLogic)", direction);

	// 正規化するとよい
	return direction;
}

// マウスのスクリーン座標からワールド座標(XZ平面:y=0)への投影位置の取得
glm::vec2 Game::Utils::ProjectScreenToLogicXZPlane(const glm::vec2& screenMousePos, const eNsGfxRender::RenderContext& renderContext)
{
	// Screen coordinates → NDC → world ray (near/far)
	// スクリーン座標 → NDC → ワールドレイ（near/far） (マウスポインタのRAY)
	glm::vec3 nearPoint = glm::unProject(glm::vec3(screenMousePos.x, renderContext.viewport.w - screenMousePos.y, 0.0f),
		renderContext.viewMatrix, renderContext.projectionMatrix, renderContext.viewport);

	glm::vec3 farPoint = glm::unProject(glm::vec3(screenMousePos.x, renderContext.viewport.w - screenMousePos.y, 1.0f),
		renderContext.viewMatrix, renderContext.projectionMatrix, renderContext.viewport);

	// 正規化の安全性を高める処理を実装する必要がある
	// nan(not a Number)が入る可能性がある.
	// nan(not a Number)が入る可能性がある.
	// nan(not a Number)が入る可能性がある.
	// マウスレイ （のベクトル)
	glm::vec3 rayDir = glm::normalize(farPoint - nearPoint);

	// 0除算防止
	if (rayDir.y == 0.0f) return glm::vec2(0.0f);
	// nearPoint.y + t * rayDir.y = (y=0)
	float t = -nearPoint.y / rayDir.y;
	// 地面(Y=0)との交差点 
	glm::vec3 intersection = nearPoint + t * rayDir;

	glm::vec2 mouseWorldXZ(intersection.x, intersection.z);

	// DebugUtils::LogVector_string("SpatialTransformUtils.cpp(convert)", mouseWorldXZ);

	return mouseWorldXZ;
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