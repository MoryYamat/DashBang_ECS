#pragma once

#include <iostream>
#include <memory>
#include <string>

// fwd
namespace ddknd::internal::platform::glfw
{
    struct CallbackState;
}

namespace ddknd::window
{
    class Window;

    namespace detail
    {
        void onFramebufferResize(Window& window, int width, int height);

        ddknd::internal::platform::glfw::CallbackState& glfwCallbackState(Window& window);
    } // namespace detail

} // namespace ddknd::window

namespace ddknd::input
{
    class IInputBackend;
    std::unique_ptr<ddknd::input::IInputBackend> CreateGlfwInputBackend(ddknd::window::Window& w);
} // namespace ddknd::input

namespace ddknd::window
{
    class GlfwContext
    {
      public:
        GlfwContext();
        ~GlfwContext();

        GlfwContext(const GlfwContext&) = delete;
        GlfwContext& operator=(const GlfwContext&) = delete;

      private:
    };

    // Window(const GlfwContext& glfw, int width, int height, std::string name)
    class Window
    {
      public:
        Window(const GlfwContext& glfw, int width, int height, std::string name);

        ~Window();

        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&& o) noexcept;
        Window& operator=(Window&& o) noexcept;

        bool ShouldClose() const;

        void PollEvents();
        void SwapBuffers();

        float aspectRatio() const
        {
            if (height_ == 0)
            {
                return 1.0f;
            }

            // std::cerr << "framebuffer resized: {}"<<  width_ << "x" <<  height_ << "\n";
            return static_cast<float>(width_) / static_cast<float>(height_);
        }

        int GetWidth() const {return width_;}
        int GetHeight() const {return height_;}

      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
        const GlfwContext& glfw_;

        int width_;
        int height_;

        void init(std::string name);

        void* nativeHandle() const;

        friend std::unique_ptr<ddknd::input::IInputBackend> ddknd::input::CreateGlfwInputBackend(
            ddknd::window::Window& w);

        friend void detail::onFramebufferResize(Window& window, int width, int height);

        friend ddknd::internal::platform::glfw::CallbackState& detail::glfwCallbackState(Window& window);
    };
} // namespace ddknd::window