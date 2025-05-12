#include "InitLogicTransformFromModel.h"

#include "Debug/DebugUtils.h"

#include <iostream>

// Initialize 2D logical data from the position, rotation, and scale of a 3D model
Logic2DTransformComponent GameInit::LogicTransform::InitLogic2DTransformFromModel(const TransformComponent& transformComp, const ModelData& modelData)
{
	Logic2DTransformComponent logic;

	// 位置データの初期化
	logic.positionXZ = glm::vec2(transformComp.position.x, transformComp.position.z);

	// 回転データの初期化 (どうやってモデル正面を得るか)
	//logic.rotation = glm::radians(transformComp.rotation.y);
	logic.rotation = transformComp.rotation.y;
	logic.UpdateDirectionFromRotation();

	// Get logical XZ size from model size and drawing scale
	logic.scale = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);

	//std::cout << "[InitLogicTransformSystem.cpp]: Logic Position: x. " << logic.positionXZ.x << " z. " << logic.positionXZ.y << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical front vector " << logic.front.x << std::endl;
	//std::cout << "[InitLogicTransformSystem.cpp]: logical right vector " << logic.right.x << std::endl;
	
	DebugUtils::GeneralLog("InitLogicTransformFromModel.cpp", 
		"Logical 2D coordinates were successfully initialized based on model drawing coordinates. ");

	return logic;
}


glm::vec2 GameInit::LogicTransform::GetModelXZSizeWithScale(const TransformComponent& transformComp, const ModelData& modelData)
{
	glm::vec2 logicalXZSize = glm::vec2(1.0f);

	// モデルデータからサイズを取得
	glm::vec3 rowModelSize = modelData.GetSize();

	// XZサイズをスケール付きで計算
	if (rowModelSize.x > 0.0001f && rowModelSize.z > 0.0001f)
	{
		logicalXZSize.x = transformComp.scale.x * rowModelSize.x;
		logicalXZSize.y = transformComp.scale.z * rowModelSize.z;
	}
	else
	{
		logicalXZSize = glm::vec2(1.0f);
		DebugUtils::GeneralLog("InitLogicTransformFromModel(scale)", "Probably an error occurred while getting/setting the scale.");
	}

	return logicalXZSize;
}





float  GameInit::LogicTransform::EstimateRadiusFromModelXZ(
	const TransformComponent& transformComp,
	const ModelData& modelData,
	RadiusEstimateStrategy strategy
)
{
	if (strategy == RadiusEstimateStrategy::MaxAxis)
	{
		glm::vec2 worldSize = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);
		return std::max(worldSize.x, worldSize.y) * 0.5f;
	}
	else if (strategy == RadiusEstimateStrategy::MinAxis)
	{
		glm::vec2 worldSize = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);
		return std::min(worldSize.x, worldSize.y) * 0.5f;
	}
	else if (strategy == RadiusEstimateStrategy::Diagonal)
	{
		glm::vec2 worldSize = GameInit::LogicTransform::GetModelXZSizeWithScale(transformComp, modelData);
		return glm::length(worldSize) * 0.5f;
	}
	else
	{
		DebugUtils::GeneralLog("InitLogicTransformFromModel.cpp(Radius)", "Maybe radius initialization failed");
		return 0.5f;

	}
}