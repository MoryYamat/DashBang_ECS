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

};