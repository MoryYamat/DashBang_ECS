#include "app.h"

#include "window/window.h"
#include "input/input.h"

namespace app
{
    App::App(int w, int h) : w_(w), h_(h)
    {

    }

    App::~App()
    {

    }

    bool App::Init()
    {
        glfwCtx_ = std::make_unique<ddknd::window::GlfwContext>();
        window_ = std::make_unique<ddknd::window::Window>(*glfwCtx_, w_, h_, "app");

        inputBackend_ = ddknd::input::CreateGlfwInputBackend(*window_);
        inputSys_ = std::make_unique<ddknd::input::InputSystem>(*inputBackend_);

        return true;
    }

    void App::Loop()
    {
        isRunning_ = true;
        while(isRunning_ && !window_->ShouldClose())
        {
            inputSys_->Update();
            window_->PollEvents();
            window_->SwapBuffers();
            if(inputSys_->isPressing(ddknd::input::Key::Escape))
                isRunning_ =false;
        }
    }

    void App::Shutdown()
    {

    }

} // namespace app