#pragma once

struct GLFWwindow;

namespace ddknd::internal::platform::glfw
{
    struct CallbackState
    {
        void* framebufferUser = nullptr;
        void (*framebufferSize)(void*, int, int) = nullptr;

        void* keyUser = nullptr;
        void (*key)(void*, int, int, int, int) = nullptr;

        void* cursorUser = nullptr;
        void (*cursor)(void*, double, double) = nullptr;

        void* mouseButtonUser = nullptr;
        void (*mouseButton)(void*, int, int, int) = nullptr;

        void* scrollUser = nullptr;
        void (*scroll)(void*, double, double) = nullptr;
    };
}