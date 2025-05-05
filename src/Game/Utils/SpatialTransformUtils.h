// Space transformation utility functions for game logic
// ゲームロジック用の空間変換系Utility関数群
#pragma once

#include <GLM/glm.hpp>

#include "Graphics/Renderer/RenderContext.h"

namespace GameUtils::SpatialTransform
{
	// マウスポインタ(スクリーン座標)から、論理XZ平面上でキャラクターを原点とした相対位置ベクトルを得る
	// Get the relative position vector from the mouse pointer (screen coordinates) to the character's origin on the logical XZ plane
	glm::vec2 GetLogicDirectionFromCursorToTarget(const glm::vec2& screenMousePos, const RenderContext& renderContext,
		const glm::vec2& playerLogicPosXZ // Character position in logical coordinates
		);

	// スクリーン座標(マウスポインタなど)から、論理XZ平面での座標を得る
	glm::vec2 ProjectScreenToLogicXZPlane(const glm::vec2& screenMousePos, const RenderContext& renderContext);
}