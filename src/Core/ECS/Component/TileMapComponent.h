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

};