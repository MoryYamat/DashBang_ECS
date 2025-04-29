#include "WindowManager.h"

#include <iostream>

GLFWwindow* WindowManager::sWindow = nullptr;
int WindowManager::sWidth = 1280;
int WindowManager::sHeight = 720;

bool WindowManager::Initialize(int width, int height, const char* title)
{
	if (!glfwInit())
	{
		std::cerr << "[WindowManager.cpp (Initialize)]: Failed to initialize GLFW" << std::endl;
		return false;
	}

	// set window varialbles
	sWidth = width;
	sHeight = height;

	// create window
	sWindow = glfwCreateWindow(sWidth, sHeight, title, nullptr, nullptr);

	if (!sWindow)
	{
		std::cerr << "[WindowManager.cpp (Initialize)]: Failed to initialize GLFW window" << std::endl;
		return false;
	}
	glfwMakeContextCurrent(sWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[WindowManager.cpp (Initialize)]: Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetFramebufferSizeCallback(sWindow, framebufferSizeCallback);

	// 
	UpdateViewport();

	return true;
}

void WindowManager::Shutdown()
{
	if (sWindow)
	{
		glfwDestroyWindow(sWindow);
		sWindow = nullptr;
	}

	glfwTerminate();
}


void WindowManager::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	SetSize(width, height);
}

void WindowManager::SetSize(int width, int height)
{
	sWidth = width;
	sHeight = height;
	//
	UpdateViewport();
}

//
void WindowManager::UpdateViewport()
{
	glViewport(0, 0, sWidth, sHeight);

	// For Debugging
	//std::cout << "[WindowManager.cpp]: view port updated to: Width:[ " << sWidth << " ]  Height:[ " << sHeight << " ] " << std::endl;
}

void WindowManager::CaptureMouse()
{
	glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowManager::ReleaseMouse()
{
	glfwSetInputMode(sWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}