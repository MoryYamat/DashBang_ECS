#include "ddknd/input/input.h"
#include "internal/glfw_internal/glfw_internal.h"
#include "ddknd/window/window.h"

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

        int toGLFWMouseButton(MouseButton mouseButton)
        {
            using MB = MouseButton;

            switch(mouseButton)
            {
                case MB::LEFT_CLICK:
                    return GLFW_MOUSE_BUTTON_LEFT; 
                case MB::RIGHT_CLICK:
                    return GLFW_MOUSE_BUTTON_RIGHT;
                case MB::MIDDLE_CLICK:
                    return GLFW_MOUSE_BUTTON_MIDDLE;
                default:
                    return GLFW_MOUSE_BUTTON_LAST;
            }
        }
    } // namespace

    class GlfwInputBackend final : public IInputBackend
    {
      private:
        using CallbackState = ::ddknd::internal::platform::glfw::CallbackState;
        struct MouseInternal
        {
            bool first = true;
            double lastX = 0.0;
            double lastY = 0.0;

            double x = 0.0;
            double y = 0.0;
            double deltaX = 0.0;
            double deltaY = 0.0;

            double wheelY = 0.0;
        };

      public:
        explicit GlfwInputBackend(GLFWwindow* window, CallbackState& callbacks) : window_(window)
        {
            //
            callbacks.keyUser = this;
            callbacks.key = [](void* user, int key, int scancode, int action, int mods)
            {
                auto* self = static_cast<GlfwInputBackend*>(user);
                self->onKey(key, scancode, action, mods);
            };

            callbacks.cursorUser = this;
            callbacks.cursor = [](void* user, double x, double y)
            {
                auto* self = static_cast<GlfwInputBackend*>(user);
                self->onCursorPosition(x, y);
            };

            callbacks.scrollUser = this;
            callbacks.scroll = [](void* user, double xoffset, double yoffset)
            {
                auto* self = static_cast<GlfwInputBackend*>(user);
                self->onScroll(xoffset, yoffset);
            };

            callbacks.mouseButtonUser = this;
            callbacks.mouseButton = [](void* user, int button, int action, int mods)
            {
                auto* self = static_cast<GlfwInputBackend*>(user);
                self->onMouseButton(button, action, mods);
            };

            // register callback to glfw
            glfwSetKeyCallback(window_, &GlfwInputBackend::key_callback);
            glfwSetCursorPosCallback(window_, GlfwInputBackend::cursor_position_callback);
            glfwSetScrollCallback(window_, GlfwInputBackend::scroll_callback);
            glfwSetMouseButtonCallback(window_, GlfwInputBackend::mouse_button_callback);

            // settings
            if (glfwRawMouseMotionSupported()) // mouse acceleration
            {
                glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
            }

            glfwSetInputMode(window_, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        ~GlfwInputBackend() override
        {
            if(window_)
            {
                glfwSetKeyCallback(window_, nullptr);
                glfwSetCursorPosCallback(window_, nullptr);
                glfwSetScrollCallback(window_, nullptr);
                glfwSetMouseButtonCallback(window_, nullptr);
            }
        }

        void Update() override
        {
            glfwPollEvents();

            mouseFrame_.x = mouseAccum_.x;
            mouseFrame_.y = mouseAccum_.y;
            mouseFrame_.deltaX = mouseAccum_.deltaX;
            mouseFrame_.deltaY = mouseAccum_.deltaY;
            mouseFrame_.wheelY = mouseAccum_.wheelY;

            // Rest delta
            mouseAccum_.deltaX = 0.0;
            mouseAccum_.deltaY = 0.0;
            mouseAccum_.wheelY = 0.0;

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

        bool IsMouseButtonDown(MouseButton mouseButton) const override
        {
            const int glfwMouseButton = toGLFWMouseButton(mouseButton);
            if(glfwMouseButton < 0 || glfwMouseButton > GLFW_MOUSE_BUTTON_LAST)
            {
                return false;
            }

            const int state = mouseButtons_[static_cast<std::size_t>(mouseButton)];
            return state == GLFW_PRESS || state == GLFW_REPEAT;
        }

      private:
        GLFWwindow* window_;

        std::array<int, GLFW_KEY_LAST + 1> keys_{};
        std::array<int, GLFW_MOUSE_BUTTON_LAST + 1> mouseButtons_{};
        MouseState mouseFrame_{};
        MouseInternal mouseAccum_{};

        void onKey(int key, int scancode, int action, int mods)
        {
            (void)scancode;
            (void)mods;

            if (key < 0 || key > GLFW_KEY_LAST)
            {
                return;
            }

            keys_[static_cast<std::size_t>(key)] = action;
        }

        void onCursorPosition(double x, double y)
        {
            MouseInternal& mouse = mouseAccum_;

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

        void onScroll(double xoffset, double yoffset)
        {
            (void) xoffset;
            mouseAccum_.wheelY += yoffset;
        }

        void onMouseButton(int button, int action, int mods)
        {
            (void) mods;
            if(button < 0 || button > GLFW_MOUSE_BUTTON_LAST)
            {
                return;
            }
            mouseButtons_[static_cast<std::size_t>(button)] = action;
        }

        static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            using CallbackState = ddknd::internal::platform::glfw::CallbackState;

            auto* state = static_cast<CallbackState*>(glfwGetWindowUserPointer(window));

            if (state == nullptr || state->key == nullptr)
            {
                return;
            }

            state->key(state->keyUser, key, scancode, action, mods);
        }

        static void cursor_position_callback(GLFWwindow* window, double x, double y)
        {
            using CallbackState = ddknd::internal::platform::glfw::CallbackState;

            auto* state = static_cast<CallbackState*>(glfwGetWindowUserPointer(window));

            if (state == nullptr || state->cursor == nullptr)
            {
                return;
            }

            state->cursor(state->cursorUser, x, y);
        }

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            using CallbackState = ddknd::internal::platform::glfw::CallbackState;

            auto state = static_cast<CallbackState*>(glfwGetWindowUserPointer(window));

            if(state == nullptr || state->scroll == nullptr)
            {
                return;
            }

            state->scroll(state->scrollUser, xoffset, yoffset);
        }

        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
        {
            using CallbackState = ddknd::internal::platform::glfw::CallbackState;

            auto state = static_cast<CallbackState*>(glfwGetWindowUserPointer(window));

            if(state == nullptr || state->mouseButton == nullptr)
            {
                return;
            }

            state->mouseButton(state->mouseButtonUser, button, action, mods);
        }
    };

    std::unique_ptr<ddknd::input::IInputBackend> CreateGlfwInputBackend(ddknd::window::Window& w)
    {
        auto* handle = static_cast<GLFWwindow*>(w.nativeHandle());

        auto& callbacks = ddknd::window::detail::glfwCallbackState(w);

        return std::make_unique<GlfwInputBackend>(handle, callbacks);
    }
} // namespace ddknd::input
