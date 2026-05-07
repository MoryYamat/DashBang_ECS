#pragma once

#include "input/input.h"
#include "window/window_fwd.h"
#include "input/input_fwd.h"
#include "graphics/graphics_fwd.h"
#include "io/io_fwd.h"


#include <memory>

// fwd
namespace ddknd::component
{
    struct DebugCameraComponent;
} 

namespace app
{
    class App
    {
        public:
            App();
            ~App();

            App(const App&) = delete;
            App& operator=(App&) = delete;

            App(App&&) = delete;
            App& operator=(App&&) = delete;

            bool Init(int w, int h);
            void Loop();
            void Shutdown();


        private:
            bool isRunning_ = false;
            std::unique_ptr<ddknd::window::Window> window_;
            std::unique_ptr<ddknd::window::GlfwContext> glfwCtx_;
            std::unique_ptr<ddknd::graphics::IRendererBackend> rendererBackend_;
            std::unique_ptr<ddknd::io::IPathResolver> vfs_;
            std::unique_ptr<ddknd::input::IInputBackend> inputBackend_;
            std::unique_ptr<ddknd::input::DeviceInput> deviceInput_;

            std::unique_ptr<ddknd::graphics::RendererSystem> renderSys_;

            std::unique_ptr<::ddknd::component::DebugCameraComponent> cam_;

            std::unique_ptr<::ddknd::input::InputMapping> inputMapping_;
            std::unique_ptr<::ddknd::input::ActionInputSystem> inputSys_;
    };
}// namespace app