#include "Window.h"

#include <iostream>

Window::Window()
{}

Window::~Window() {
	Shutdown();
}

bool Window::Initialize(int width, int height, const std::string& title)
{
	if (!glfwInit())
	{
		std::cerr << "[Window.cpp]: Failed to initialize GLFW\n";
		return false;
	}

	mWidth = width;
	mHeight = height;

	mWindow = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
	if (!mWindow)
	{
		std::cerr << "[Window.cpp]: Failed to create window\n";
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(mWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "[WindowManager.cpp (Initialize)]: Failed to initialize GLAD" << std::endl;
		return false;
	}

	glfwSetWindowUserPointer(mWindow, this);

	glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height) {
		auto self = static_cast<Window*>(glfwGetWindowUserPointer(window));
		if (self)  self->SetSize(width, height);
		});

	UpdateViewport();

	return true;
}

void Window::Shutdown()
{
	if (mWindow)
	{
		glfwDestroyWindow(mWindow);
		mWindow = nullptr;
	}
}

void Window::UpdateViewport()
{
	glViewport(0, 0, mWidth, mHeight);
}

void Window::SetSize(int width, int height)
{
	mWidth = width;
	mHeight = height;
	UpdateViewport();
}

void Window::CaptureMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Window::ReleaseMouse()
{
	glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}