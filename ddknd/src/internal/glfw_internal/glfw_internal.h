#pragma once

struct GLFWwindow;

namespace ddknd::internal::platform::glfw
{
    /**
     * @brief Dispatch state for GLFW callbacks
     * 
     * GLFW provides a single user-pointer slot per window. CallbackState is
     * stored in that slot and routes each callback category to its corresponding
     * non-owning user pointer and handler function.
     *
     * The user pointers must remain valid while their handlers are registered
     */
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
} // namespace ddknd::internal::platform::glfw