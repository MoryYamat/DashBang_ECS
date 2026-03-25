#include "window/window.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <spdlog/spdlog.h>

#include <memory>
#include <string>


namespace ddknd::window
{
    GlfwContext::GlfwContext()
    {
        if(!glfwInit())
        {
            spdlog::critical("glfwInit failed");
            std::terminate();
        }
    }

    GlfwContext::~GlfwContext()
    {
        glfwTerminate();
    }

    struct Window::Impl
    {
        GLFWwindow* handle =nullptr;
        bool initialized = false;
    };

    Window::Window(const GlfwContext& glfw, int width, int height, std::string name)
        : impl_(std::make_unique<Impl>()), glfw_(glfw), width_(width), height_(height)
    {
        init(std::move(name));
    }

	Window::~Window()
	{
		if (impl_ && impl_->handle)
		{
			glfwDestroyWindow(impl_->handle);
			impl_->handle = nullptr;
		}

		spdlog::info("window destructed");
	}

	Window::Window(Window&& o) noexcept
		: impl_(std::move(o.impl_)), glfw_(o.glfw_), width_(o.width_), height_(o.height_)
	{
		o.width_ = 0;
		o.height_ = 0;
	}

	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(impl_->handle);
	}

	void Window::PollEvents()
	{
		glfwPollEvents();
	}
	void Window::SwapBuffers()
	{
		glfwSwapBuffers(impl_->handle);
	}

	void Window::init(std::string name)
	{
		if (impl_->handle)
			return;

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		impl_->handle = glfwCreateWindow(width_, height_, name.c_str(), NULL, NULL);
		if (!impl_->handle)
		{
			spdlog::critical("glfwCreateWindow failed");
			std::terminate();
		}

		glfwMakeContextCurrent(impl_->handle);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			spdlog::critical("gladLoadGLLoader failed");
			std::terminate();
		}

		spdlog::info("success init");
		impl_->initialized = true;
	}

	void* Window::nativeHandle() const
	{
		return impl_->handle;
	}

}// namespace ddknd::window