// TileMap base/unit data

#pragma once

#include <glm/glm.hpp>

#include <vector>

struct Tile
{
	bool isWalkable = true;
	int tileType = 0;
};


// Data for the entire TileMap

struct TileMapComponent
{
	int numCols;// 横の枚数
	int numRows;// 縦の枚数
	float tileSize = 1.0f;// タイルの1辺のサイズ
	glm::vec2 origin = { 0, 0 };// タイルの左下の`ワールド座標`

	std::vector<std::vector<Tile>> tiles;

	// タイルの左下の`ワールド座標`を取得(改善必要（ワールド座標における回転と論理(描画)位置の反映が必要）)
	// 論理的な中心を原点にするため、左下にオフセット
	glm::vec2 GetOrigin(glm::vec2 position)
	{
		return glm::vec2(position.x + (numCols * tileSize * -0.5f), position.y + (numRows * tileSize * -0.5f));
	}
};