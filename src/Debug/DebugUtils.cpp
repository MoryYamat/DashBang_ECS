#include "DebugUtils.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


// Fixed Function Pipeline
void DebugUtils::DebugDraw::DrawCross(const glm::vec3& pos, float size, const glm::vec3& color)
{
	glUseProgram(0); // shader ������
	glDisable(GL_DEPTH_TEST);// �e�X�g�[�x
	glLineWidth(2.0f);// ���̑���

	glBegin(GL_LINES);
	glColor3f(color.r, color.g, color.b);// ��

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

void DebugUtils::DebugDraw::DrawFilledQuad(const glm::vec2& center, float size, glm::vec3& color)
{
	float halfSize = size * 0.5f;

	glm::vec3 v0(center.x - halfSize, 0.0f, center.y - halfSize);
	glm::vec3 v1(center.x + halfSize, 0.0f, center.y - halfSize);
	glm::vec3 v2(center.x + halfSize, 0.0f, center.y + halfSize);
	glm::vec3 v3(center.x - halfSize, 0.0f, center.y + halfSize);

	glUseProgram(0);
	glDisable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ���u�����h�ݒ�

	glColor4f(color.r, color.g, color.b, 0.3f);

	// ���_�ݒ�
	glBegin(GL_TRIANGLES);
	glVertex3f(v0.x, v0.y, v0.z);
	glVertex3f(v1.x, v1.y, v1.z);
	glVertex3f(v2.x, v2.y, v2.z);

	glVertex3f(v0.x, v0.y, v0.z);
	glVertex3f(v2.x, v2.y, v2.z);
	glVertex3f(v3.x, v3.y, v3.z);

	// for debugging
	// DebugUtils::LogVector_string("DebugUtils.cpp(vertex)", v0);

	glEnd();
	glDisable(GL_BLEND);
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
		points.push_back(glm::vec3(centerXZ.x + x, 0.01f, centerXZ.y + z)); // Y=0.01�ŕ�������
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


}

void DebugUtils::DebugDraw::DrawFilledCircle2D(const glm::vec2& centerXZ, float radius, const glm::vec4& color, int segments)
{
	const float y = 0.05f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex3f(centerXZ.x, y, centerXZ.y); // ���S



	for (int i = 0; i <= segments; ++i)
	{
		float theta = glm::two_pi<float>() * i / segments;
		glm::vec2 point = centerXZ + radius * glm::vec2(std::cos(theta), std::sin(theta));
		glVertex3f(point.x, y, point.y);
	}
	glEnd();
	glDisable(GL_BLEND);
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



void DebugUtils::DebugDraw::DrawLine2D(const glm::vec2& a, const glm::vec2 b, const glm::vec3& color)
{
	// OpenGL Immediate Mode (for debug only)
	glColor3f(color.r, color.g, color.b);
	glBegin(GL_LINES);
	glVertex3f(a.x, 0.1f, a.y); // Y=0.1f (�n�ʂ��班���������ĕ`��)
	glVertex3f(b.x, 0.1f, b.y);
	glEnd();
}

void DebugUtils::DebugDraw::DrawSector2D(const glm::vec2 centerXZ, const glm::vec2 direction, const float radius, const float angle, const glm::vec3& color)
{
	const int segments = 20;
	const float halfAngle = angle / 2.0f;

	// ��p�x�idirection����j
	float baseAngle = std::atan2(direction.y, direction.x);

	std::vector<glm::vec2> points;
	points.push_back(centerXZ); // ��`�̒��S

	// �~�ʕ����̓_�𐶐�
	for (int i = 0; i <= segments; ++i)
	{
		float t = (float)i / (float)segments;
		float theta = baseAngle - halfAngle + t * angle;

		glm::vec2 point = centerXZ + radius * glm::vec2(std::cos(theta), std::sin(theta));
		points.push_back(point);
	}

	// ���`��F���S�����̉���
	for (size_t i = 1; i < points.size(); ++i)
	{
		DrawLine2D(centerXZ, points[i], color); // ���S�����̐�
	}

	// ���`��F�~�ʂ�����
	for (size_t i = 1; i < points.size() - 1; ++i)
	{
		DrawLine2D(points[i], points[i + 1], color); // ���̐�
	}
}

void DebugUtils::DebugDraw::DrawFilledSector2D(const glm::vec2 centerXZ, const glm::vec2 direction, const float radius, const float angle, const glm::vec4& color)
{
	const int segments = 32;
	const float halfAngle = angle / 2.0f;
	const float baseAngle = std::atan2(direction.y, direction.x);
	const float y = 0.05f; // �n�ʂ��班����������

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(color.r, color.g, color.b, color.a);
	glVertex3f(centerXZ.x, y, centerXZ.y); // ��̒��S

	for (int i = 0; i <= segments; ++i)
	{
		float theta = baseAngle - halfAngle + i * (angle / segments);
		glm::vec2 point = centerXZ + radius * glm::vec2(std::cos(theta), std::sin(theta));
		glVertex3f(point.x, y, point.y);
	}
	glEnd();
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}