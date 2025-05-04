// Space transformation utility functions for game logic
// ゲームロジック用の空間変換系Utility関数群
#pragma once

#include <GLM/glm.hpp>

namespace GameUtils::SpatialTransform
{
	// マウスポインタ(スクリーン座標)から、論理XZ平面上でキャラクターを原点とした相対位置ベクトルを得る
	// Get the relative position vector from the mouse pointer (screen coordinates) to the character's origin on the logical XZ plane
	glm::vec2 MouseTo2DLogicConverter(
		const glm::vec2& screenMousePos,// mouse position
		const glm::mat4& viewMatrix,//  view matrix
		const glm::mat4& projectionMatrix,// projection matrix
		const glm::vec4& viewport,// viewport setting
		const glm::vec2& playerLogicPosXZ // Character position in logical coordinates
		);
}