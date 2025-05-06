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

	// �^�C���̍�����`���[���h���W`���擾(���P�K�v�i���[���h���W�ɂ������]�Ƙ_��(�`��)�ʒu�̔��f���K�v�j)
	// �_���I�Ȓ��S�����_�ɂ��邽�߁A�����ɃI�t�Z�b�g
	glm::vec2 GetOrigin(glm::vec2 position)
	{
		return glm::vec2(position.x + (numCols * tileSize * -0.5f), position.y + (numRows * tileSize * -0.5f));
	}
};