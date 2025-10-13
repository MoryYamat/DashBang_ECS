#pragma once

#include <GLM/glm.hpp>


namespace Game::Character::Animation::Query
{
	// skill animation の phase time を論理系で流用する
	// 通常攻撃はtarget方式で当たり判定を副作用で行うような方式にはしないかもしれない
	// そのあたりはmixamoのアニメーション的限界を考え、ゲームデザイン詳細を詰める
	// その場合でも対応できるアニメーション再生の構造が必要

	// locomotion
	struct LocomotionAnimQueryComponent
	{
		// movement
		bool isMoving = false;
		float moveSpeed = 0.0f;
		float normalizedSpeed = 0.0f;
		glm::vec2 moveDirWorld = { 0,1 };
		glm::vec2 facingDirWorld = { 0,1 };
		float moveYawLocal = 0.0f;// facing基準の角度 [-pi, pi]
		bool isLocomotionActive = false;

		// 
		bool valid = false;
	};
}