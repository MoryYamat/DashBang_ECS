// Initialize logical data from drawing data

#pragma once

#include "DataTypes/ModelData.h"
#include "Core/ECS/Component/Logic2DTransformComponent.h"

#include "Core/ECS/Component/TransformComponent.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include <string>

namespace GameInit::LogicTransform
{
	// 3D -> 2D
	Logic2DTransformComponent InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData);

	// 3D -> 3D

	// 2D -> 2D
	//===============================
	
	// Get logical XZ size from model size, taking into account scale
	// // スケールを考慮してモデルサイズから論理XZサイズを取得します
	glm::vec2 GetModelXZSizeWithScale(const TransformComponent& transformComp, const ModelData& modelData);


}

// For Player's 2DCircle Collision form
namespace GameInit::LogicTransform
{
	enum class RadiusEstimateStrategy
	{
		MaxAxis,  // Use the larger of width or depth
		MinAxis,  // Use the smaller of width or depth
		Diagonal      // Use the average of width and depth

	};

	// For initializing collision shape 
	// When the maximum axis of the model's size on the XZ plane is the radius
	// モデルのXZ平面における大きさの最大軸を半径にする場合

	float EstimateRadiusFromModelXZ(
		const TransformComponent& transformComp,
		const ModelData& modelData,
		RadiusEstimateStrategy strategy = RadiusEstimateStrategy::MaxAxis
	);
}

// for Rectangle collision shape initialization with scale/rotation
namespace GameInit::LogicTransform
{

}