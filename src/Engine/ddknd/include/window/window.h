#pragma once

#include <string>
#include <memory>

namespace ddknd::window
{
    class Window;
}// namespace ddknd::window

namespace ddknd::input
{
    class IInputBackend;
    std::unique_ptr<ddknd::input::IInputBackend> CreateGlfwInputBackend(const ddknd::window::Window& w);
}// namespace ddknd::input

namespace ddknd::window
{
    class GlfwContext
    {
        public:
            GlfwContext();
            ~GlfwContext();

            GlfwContext(const GlfwContext&) = delete;
            GlfwContext& operator=(const  GlfwContext&) = delete;

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
            Window(Window&& o)noexcept;
            Window& operator=(Window&& o)noexcept;

        bool ShouldClose() const;

            void PollEvents();
            void SwapBuffers();



        private:
            struct Impl;
            std::unique_ptr<Impl> impl_;
            const GlfwContext& glfw_;

            int width_;
            int height_;

            void init(std::string name);

            void* nativeHandle() const;
            
            friend std::unique_ptr<ddknd::input::IInputBackend> ddknd::input::CreateGlfwInputBackend(const ddknd::window::Window& w);
    };
}// namespace ddknd::window