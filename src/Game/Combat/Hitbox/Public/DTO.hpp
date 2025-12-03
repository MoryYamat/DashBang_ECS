// Skill名 に対する Hitbox(判定) を 定義するために作者が定義するDTO
// SkillRuntimeDTO と 統合するかは一考の余地あり
#pragma once

#include <string>

namespace Game::Combat::HitBox
{
	struct HitBoxDTO
	{
		std::string name;		// hitbox name(!= skill name), 
		std::string shape;		// "Circle", "Sector",...
		float radius;			// circle用
		float angle;			// Sector 用 (degree)
		float length;			// Square/Sector用
		float vert;				// Rect 正面方向
		float horizon;			// Rect 水平方向
		float offsetX;			// owner からのオフセット
		float offsetY;
	};
}

// testができたら std::string sync;を追加 