#pragma once

#include "window/window_fwd.h"
#include "input/input_fwd.h"
#include "graphics/graphics_fwd.h"
#include "io/io_fwd.h"


#include <memory>

// fwd
namespace ddknd::component
{
    struct CameraComponent;
} 

namespace app
{
    class App
    {
        public:
            App(int w, int h);
            ~App();

            App(const App&) = delete;
            App& operator=(App&) = delete;

            App(App&&) = delete;
            App& operator=(App&&) = delete;

            bool Init();
            void Loop();
            void Shutdown();


        private:
            int h_ = 0, w_ = 0;
            bool isRunning_ = false;
            std::unique_ptr<ddknd::window::Window> window_;
            std::unique_ptr<ddknd::window::GlfwContext> glfwCtx_;
            std::unique_ptr<ddknd::graphics::IRendererBackend> rendererBackend_;
            std::unique_ptr<ddknd::io::IPathResolver> vfs_;
            std::unique_ptr<ddknd::input::IInputBackend> inputBackend_;
            std::unique_ptr<ddknd::input::InputSystem> inputSys_;

            std::unique_ptr<ddknd::graphics::RendererSystem> renderSys_;

            std::unique_ptr<::ddknd::component::CameraComponent> cam_;
    };
}// namespace app