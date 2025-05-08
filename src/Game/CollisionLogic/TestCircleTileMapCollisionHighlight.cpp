#include "TestCircleTileMapCollisionHighlight.h"

#include "Game/CollisionLogic/CollisionUtils.h"

#include "Debug/DebugUtils.h"

void CollisionUtils::TestCircleTileMapCollisionHighlight(
	const CollisionComponent& collisionComp,
	const TileMapComponent& tileMapComp
)
{
	// �~�łȂ���΃X�L�b�v
	if (collisionComp.collider.type != ColliderType::Circle2D)
		return;

	const glm::vec2& center = collisionComp.collider.circle2D.center;
	float radius = collisionComp.collider.circle2D.radius;

	// �~�̒��S��TileMap��̃��[�J�����W�ɕϊ�
	glm::vec2 relative = center - tileMapComp.origin;

	// �^�C���C���f�b�N�X�͈�(�t�߂̃^�C������)
	int colStart = static_cast<int>(std::floor((relative.x - radius) / tileMapComp.tileSize)) - 1;
	int colEnd = static_cast<int>(std::ceil((relative.x + radius) / tileMapComp.tileSize)) + 1;
	int rowStart = static_cast<int>(std::floor((relative.y - radius) / tileMapComp.tileSize)) - 1;
	int rowEnd = static_cast<int>(std::ceil((relative.y + radius) / tileMapComp.tileSize)) + 1;

	// �͈̓N�����v(���S�Ƀ^�C���A�N�Z�X)
	colStart = std::max(0, colStart);
	colEnd = std::min(tileMapComp.numCols - 1, colEnd);
	rowStart = std::max(0, rowStart);
	rowEnd = std::min(tileMapComp.numRows - 1, rowEnd);

	for (int row = rowStart; row <= rowEnd; ++row)
	{
		for (int col = colStart; col <= colEnd; ++col)
		{
			glm::vec2 tileMin = tileMapComp.origin + glm::vec2(col, row) * tileMapComp.tileSize;
			glm::vec2 tileMax = tileMin + glm::vec2(tileMapComp.tileSize);

			if (CollisionUtils::CheckCircleAABBIntersection(center, radius, tileMin, tileMax))
			{
				// �������Ă���^�C����Ԃŕ`��iY��������������Ǝ��F��UP�j
				DebugUtils::DebugDraw::DrawTileOutline(tileMin, tileMax, glm::vec3(1.0f, 0.0f, 0.0f));
			}
		}
	}
}