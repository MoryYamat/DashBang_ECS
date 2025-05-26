// tile grid XZ

#pragma once

#include "Config/CanonicalDefaults.h"

#include "Math/Logic/LogicMathUtils.h"

#include <GLM/glm.hpp>

struct TileData
{
	bool isWalkable = true;
	int tileType = 0;
};

// Y = 0にある論理平面のデータ構造
struct TileGridXZComponent
{
	int numCols;
	int numRows;
	float tileSize = 1.0f;

	// origin は(0, 0, 0)で Zがrow方向，Xがcol方向に広がる 
	glm::vec3 origin = CanonicalDefaults::kCanonicalOrigin;

	//tiles[row][col]
	std::vector<std::vector<TileData>> tiles;

	//
	inline std::pair<glm::vec3, glm::vec3> GetTileAABB(int row, int col) const
	{
		glm::vec3 tileMin = origin + glm::vec3(col * tileSize, 0.0f, row * tileSize);
		glm::vec3 tileMax = tileMin + glm::vec3(tileSize, 0.0f, tileSize);
		return { tileMin, tileMax };
	}

	//
	glm::vec3 GetTileCenter(int row, int col)
	{
		return origin + glm::vec3((col + 0.5f) * tileSize, 0.0f, (row + 0.5f) * tileSize);
	}
};