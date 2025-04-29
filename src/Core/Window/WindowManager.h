// for systems window manager

#pragma once

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

class WindowManager
{
public:
	static bool Initialize(int width, int height, const char* title);
	static void Shutdown();

	static void UpdateViewport();
	static void SetSize(int widht, int height);

	static int GetWidth() { return sWidth; }
	static int GetHeight() { return sHeight; }
	static float GetAspect() { return static_cast<float>(sWidth) / (sHeight); }
	static GLFWwindow* GetWindow() { return sWindow; }

	static void CaptureMouse();
	static void ReleaseMouse();

private:
	static GLFWwindow* sWindow;
	static int sWidth;
	static int sHeight;

	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
};