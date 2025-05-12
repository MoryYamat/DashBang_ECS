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
	int numCols;// ���̖���
	int numRows;// �c�̖���
	float tileSize = 1.0f;// �^�C����1�ӂ̃T�C�Y

	glm::vec2 origin = { 0, 0 };// �^�C���̍�����`���[���h���W`

	std::vector<std::vector<Tile>> tiles;

	inline std::pair<glm::vec2, glm::vec2> GetTileAABB(int row, int col) const
	{
		glm::vec2 tileMin = origin + glm::vec2(col, row) * tileSize;
		glm::vec2 tileMax = tileMin + glm::vec2(tileSize);
		return { tileMin, tileMax };
	}

	glm::vec2 GetTileCenter(int row, int col)
	{
		return origin + glm::vec2(col + 0.5f, row + 0.5f) * tileSize;
	}
};