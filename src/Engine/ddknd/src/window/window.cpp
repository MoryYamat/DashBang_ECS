#include "window/window.h"

#include "internal/glfw_internal/glfw_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <spdlog/spdlog.h>

#include <memory>
#include <string>

namespace
{
    using namespace ::ddknd::window;
    static void framebuffer_size_callback(GLFWwindow* handle, int width, int height);

} // namespace

namespace ddknd::window
{
    GlfwContext::GlfwContext()
    {
        if (!glfwInit())
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
        using GlfwCallbackState = ::ddknd::internal::platform::glfw::CallbackState;
        GLFWwindow* handle = nullptr;
        bool initialized = false;
        GlfwCallbackState callbackState;
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

        //
        impl_->callbackState.framebufferUser = this;
        impl_->callbackState.framebufferSize = [](void* user, int width, int height)
        {
            auto* window = static_cast<Window*>(user);
            detail::onFramebufferResize(*window, width, height);
        };
        glfwSetWindowUserPointer(impl_->handle, &impl_->callbackState);
        glfwMakeContextCurrent(impl_->handle);

        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            spdlog::critical("gladLoadGLLoader failed");
            std::terminate();
        }

        int fbWidth = 0;
        int fbHeight = 0;
        glfwGetFramebufferSize(impl_->handle, &fbWidth, &fbHeight);

        glViewport(0, 0, fbWidth, fbHeight);
        detail::onFramebufferResize(*this, fbWidth, fbHeight);

        glfwSetFramebufferSizeCallback(impl_->handle, framebuffer_size_callback);

        spdlog::info("success init");
        impl_->initialized = true;
    }

    void* Window::nativeHandle() const
    {
        return impl_->handle;
    }

    namespace detail
    {
        void onFramebufferResize(Window& window, int width, int height)
        {
            window.width_ = width;
            window.height_ = height;
        }

        ddknd::internal::platform::glfw::CallbackState& glfwCallbackState(Window& window)
        {
            return window.impl_->callbackState;
        }
    } // namespace detail

} // namespace ddknd::window

namespace
{
    void framebuffer_size_callback(GLFWwindow* handle, int width, int height)
    {
        glViewport(0, 0, width, height);

        auto* state = static_cast<ddknd::internal::platform::glfw::CallbackState*>(glfwGetWindowUserPointer(handle));

        if (state == nullptr || state->framebufferSize == nullptr)
        {
            return;
        }

        state->framebufferSize(state->framebufferUser, width, height);
    }
} // namespace