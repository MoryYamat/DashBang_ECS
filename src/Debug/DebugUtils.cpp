#include "DebugUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Fixed Function Pipeline
void DebugUtils::DebugDraw::DrawCross(const glm::vec3& pos, float size, const glm::vec3& color)
{
	glUseProgram(0); // shader 無効化
	glDisable(GL_DEPTH_TEST);// テスト深度
	glLineWidth(2.0f);// 線の太さ

	glBegin(GL_LINES);
	glColor3f(color.r, color.g, color.b);// 赤

	// x line
	glVertex3f(pos.x - size, pos.y, pos.z);
	glVertex3f(pos.x + size, pos.y, pos.z);

	// z line
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(pos.x, pos.y, pos.z - size);
	glVertex3f(pos.x, pos.y , pos.z + size);

	glEnd();

	glEnable(GL_DEPTH_TEST);
}

void DebugUtils::DebugDraw::DrawQuad(const glm::vec2& center, float size, const glm::vec3& color)
{
	float halfSize = size * 0.5f;

	glm::vec3 v0(center.x - halfSize, 0.0f, center.y - halfSize);
	glm::vec3 v1(center.x + halfSize, 0.0f, center.y - halfSize);
	glm::vec3 v2(center.x + halfSize, 0.0f, center.y + halfSize);
	glm::vec3 v3(center.x - halfSize, 0.0f, center.y + halfSize);

	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glColor3f(color.r, color.g, color.b);

	glBegin(GL_LINE_LOOP);
	glVertex3f(v0.x, v0.y, v0.z);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v3.x, v3.y, v3.z);

	// for debugging
	// DebugUtils::LogVector_string("DebugUtils.cpp(vertex)", v0);

	glEnd();

	glEnable(GL_DEPTH_TEST);
}

void DebugUtils::DebugDraw::DrawCircle2D(const glm::vec2& centerXZ, float radius, const glm::vec3& color, int segments)
{
	std::vector<glm::vec3> points;
	for (int i = 0; i < segments; ++i)
	{
		float angle = (float)i / segments * 2.0f * glm::pi<float>();
		float x = std::cos(angle) * radius;
		float z = std::sin(angle) * radius;
		points.push_back(glm::vec3(centerXZ.x + x, 0.01f, centerXZ.y + z)); // Y=0.01で浮かせる
	}

	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glColor3f(color.r, color.g, color.b);
	
	glBegin(GL_LINE_LOOP);
	for (const auto& p : points)
	{
		glVertex3f(p.x, p.y, p.z);
	}
	glEnd();

	glEnable(GL_DEPTH_TEST);

}




void DebugUtils::DebugDraw::DrawTileOutline(const glm::vec2& minXZ, const glm::vec2& maxXZ, const glm::vec3& color)
{
	std::vector<glm::vec3> corners = {
	{minXZ.x, 0.05f, minXZ.y},
	{maxXZ.x, 0.05f, minXZ.y},
	{maxXZ.x, 0.05f, maxXZ.y},
	{minXZ.x, 0.05f, maxXZ.y}
	};

	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINE_LOOP);
	for (const auto& p : corners)
		glVertex3f(p.x, p.y, p.z);
	glEnd();
	glEnable(GL_DEPTH_TEST);
}