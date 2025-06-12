// Position data used for logical calculations such as collision detection

#pragma once 

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include "Common/EngineNamespaceDecl.h"

#include <glm/glm.hpp>

namespace Engine::ECS::Component::Logic2D
{
	struct Logic2DTransformComponent
	{
		//position (x, z)
		glm::vec2 positionXZ = { 0.0f, 0.0f };

		// direction (radians)
		float rotation = CanonicalDefaults::kCanonicalRotationY;// radians!

		// Front and right vectors
		glm::vec2 front = CanonicalDefaults::kCanonicalFrowardXZ;
		glm::vec2 right = CanonicalDefaults::kCanonicalRightXZ;

		// scale
		glm::vec2 scale = glm::vec2(1.0f);

		// Frontベクトルから回転を得る (radians)
		float GetRotationYFromFrontVector() const
		{
			return eNsLogic2DMath::CalcYawFromDirection(front);
		}

		// FrontベクトルからRightベクトルを得る
		void UpdateRightFromFront()
		{
			right = eNsLogic2DMath::CalcRightFromForward(front);
		}

		// 初期化時のみ　※ゲームループでは使わない
		void UpdateDirectionFromRotation()
		{
			front = glm::normalize(eNsLogic2DMath::CalcForwardFromYaw(rotation));
			right = eNsLogic2DMath::CalcRightFromYaw(rotation);
		}

	};
}
