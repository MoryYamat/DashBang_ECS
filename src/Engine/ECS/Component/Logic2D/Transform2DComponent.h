// A component that holds information in 2D world space.
// 2Dワールド空間での情報を保持するコンポーネント

#pragma once 

#include "Engine/Config/CanonicalDefaults.h"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <glm/gtx/rotate_vector.hpp>

#include <GLM/glm.hpp>

#include "Common/EngineNamespaceDecl.h"

namespace Engine::ECS::Component::Logic2D
{
	struct Transform2DComponent
	{
		glm::vec2 positionXZ = { 0.0f, 0.0f };
		float rotationY = 0.0f; // radians
		float scale = 1.0f; // uniform

		glm::vec2 front = CanonicalDefaults::kLocalForwardXZ;
		glm::vec2 right = eNsLogic2DMath::CalcRightFromForward(front);

		// front (radians前提)
		glm::vec2 GetFrontXZ() const
		{
			return eNsLogic2DMath::CalcForwardFromYaw(rotationY);// +Z基準
		}

		// right (radians前提)
		glm::vec2 GetRightXZ() const
		{
			return eNsLogic2DMath::CalcRightFromYaw(rotationY);// +Z基準
		}

		// frontに合わせて更新
		void UpdateRightFromFront()
		{
			right = eNsLogic2DMath::CalcRightFromForward(front);
		}

	};
}
