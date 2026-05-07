#include "input/input.h"
#include "window/window.h"

#include <array>
#include <iostream>

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

namespace ddknd::input
{
    namespace
    {
        int toGLFWKey(Key k)
        {
            switch (k)
            {
            case Key::A:
                return GLFW_KEY_A;
            case Key::B:
                return GLFW_KEY_B;
            case Key::C:
                return GLFW_KEY_C;
            case Key::D:
                return GLFW_KEY_D;
            case Key::E:
                return GLFW_KEY_E;
            case Key::F:
                return GLFW_KEY_F;
            case Key::G:
                return GLFW_KEY_G;
            case Key::H:
                return GLFW_KEY_H;
            case Key::I:
                return GLFW_KEY_I;
            case Key::J:
                return GLFW_KEY_J;
            case Key::K:
                return GLFW_KEY_K;
            case Key::L:
                return GLFW_KEY_L;
            case Key::M:
                return GLFW_KEY_M;
            case Key::N:
                return GLFW_KEY_N;
            case Key::O:
                return GLFW_KEY_O;
            case Key::P:
                return GLFW_KEY_P;
            case Key::Q:
                return GLFW_KEY_Q;
            case Key::R:
                return GLFW_KEY_R;
            case Key::S:
                return GLFW_KEY_S;
            case Key::T:
                return GLFW_KEY_T;
            case Key::U:
                return GLFW_KEY_U;
            case Key::V:
                return GLFW_KEY_V;
            case Key::W:
                return GLFW_KEY_W;
            case Key::X:
                return GLFW_KEY_X;
            case Key::Y:
                return GLFW_KEY_Y;
            case Key::Z:
                return GLFW_KEY_Z;
            case Key::SPACE:
                return GLFW_KEY_SPACE;
            case Key::ESCAPE:
                return GLFW_KEY_ESCAPE;
            case Key::LEFT_SHIFT:
                return GLFW_KEY_LEFT_SHIFT;
            default:
                return GLFW_KEY_LAST;
            }
        }
    } // namespace

    class GlfwInputBackend final : public IInputBackend
    {
      private:
        struct MouseInternal
        {
            bool first = true;
            double lastX = 0.0;
            double lastY = 0.0;

            double x = 0.0;
            double y = 0.0;
            double deltaX = 0.0;
            double deltaY = 0.0;
        };

      public:
        explicit GlfwInputBackend(GLFWwindow* window) : window_(window)
        {
            glfwSetWindowUserPointer(window_, this);
            glfwSetKeyCallback(window_, &GlfwInputBackend::key_callback);
            glfwSetCursorPosCallback(window_, GlfwInputBackend::curosor_position_callback);
            if (glfwRawMouseMotionSupported()) // mouse acceleration
            {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }
        }

        void Update() override
        {
            glfwPollEvents();

            mouseFrame_.x = mouseAccum_.x;
            mouseFrame_.y = mouseAccum_.y;
            mouseFrame_.deltaX = mouseAccum_.deltaX;
            mouseFrame_.deltaY = mouseAccum_.deltaY;

            // Rest delta
            mouseAccum_.deltaX = 0.0;
            mouseAccum_.deltaY = 0.0;
        }

        const MouseState& Mouse() const override
        {
            return mouseFrame_;
        }

        bool IsDown(Key k) const override
        {
            const int glfwKey = toGLFWKey(k);
            if (glfwKey < 0 || glfwKey > GLFW_KEY_LAST)
                return false;
            const int state = keys_[static_cast<std::size_t>(glfwKey)];
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

      private:
        GLFWwindow* window_;

        std::array<int, GLFW_KEY_LAST + 1> keys_{};
        MouseState mouseFrame_{};
        MouseInternal mouseAccum_{};

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            (void)scancode;
            (void)mods;

            auto* self = static_cast<GlfwInputBackend*>(glfwGetWindowUserPointer(window));
            if (!self)
            {
                spdlog::error("unexpected error");
                return;
            }
            if (key < 0)
            {
                spdlog::error("invalid key input");
                return;
            }
            self->keys_[key] = action;
        }

        static void curosor_position_callback(GLFWwindow* window, double x, double y)
        {
            auto* self = static_cast<GlfwInputBackend*>(glfwGetWindowUserPointer(window));

            if (self == nullptr)
            {
                return;
            }

            MouseInternal& mouse = self->mouseAccum_;

            if (mouse.first)
            {
                mouse.lastX = x;
                mouse.lastY = y;
                mouse.x = x;
                mouse.y = y;
                mouse.first = false;
                return;
            }

            mouse.deltaX += x - mouse.lastX;
            mouse.deltaY += y - mouse.lastY;

            mouse.lastX = x;
            mouse.lastY = y;
            mouse.x = x;
            mouse.y = y;
        }
    };

    std::unique_ptr<ddknd::input::IInputBackend> CreateGlfwInputBackend(const ddknd::window::Window& w)
    {
        std::unique_ptr<ddknd::input::IInputBackend> backend =
            std::make_unique<ddknd::input::GlfwInputBackend>(static_cast<GLFWwindow*>(w.nativeHandle()));
        return std::move(backend);
    }
} // namespace ddknd::input
