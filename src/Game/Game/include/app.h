#pragma once

#include "window/window_fwd.h"
#include "input/input_fwd.h"

#include <memory>


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
            std::unique_ptr<ddknd::window::Window> window_;
            std::unique_ptr<ddknd::window::GlfwContext> glfwCtx_;
            std::unique_ptr<ddknd::input::IInputBackend> inputBackend_;
            std::unique_ptr<ddknd::input::InputSystem> inputSys_;
    };
}// namespace app