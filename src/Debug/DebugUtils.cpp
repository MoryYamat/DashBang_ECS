#include "DebugUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Fixed Function Pipeline
void DebugUtils::DebugDraw::DrawCross(glm::vec3 pos, float size)
{
	glUseProgram(0); // shader 無効化
	glDisable(GL_DEPTH_TEST);// テスト深度
	glLineWidth(2.0f);// 線の太さ

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);// 赤

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

void DebugUtils::DebugDraw::DrawFlatCircle(glm::vec3 pos, float radius, int segments)
{
	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < segments; ++i)
	{
		float theta = (float)i / segments * 2.0f * 3.1415926f;
		float x = radius * cos(theta);
		float z = radius * sin(theta);
		glVertex3f(pos.x + x, pos.y, pos.z + z);
	}

	glEnd();

	glEnable(GL_DEPTH_TEST);
}