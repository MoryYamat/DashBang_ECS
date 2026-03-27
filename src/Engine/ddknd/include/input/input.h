#pragma once

#include <cstdint>
#include <bitset>
#include <cstddef>
#include <memory>

namespace ddknd::window
{
    class Window;
}

namespace ddknd::input
{
    // TODO: add all keys of glfws
    enum class Key : std::uint16_t
    {
        A,B,C,D,E,F,G,H,I,J,K,L,M,N,O,P,Q,R,S,T,U,V,W,X,Y,Z,
        Escape,
        Space,
        COUNT
    };

    class IInputBackend
    {
      public:
        virtual ~IInputBackend() = default;
        virtual void Update() = 0;
        virtual bool isKeyPressed(Key k) const = 0;
    };

    class InputSystem
    {
      public:
        InputSystem(IInputBackend& bc) : backend_(bc){}

        bool isPressing(Key k) const
        {
            return curr_[toIdx(k)];
        }

        void Update()
        {
            for(std::size_t i = 0; i < static_cast<std::size_t>(Key::COUNT); i++)
            {
                curr_[i] = backend_.isKeyPressed(static_cast<Key>(i));
            }
        }

      private:
        IInputBackend& backend_;

        std::bitset<static_cast<std::size_t>(Key::COUNT)> curr_;

        constexpr std::size_t toIdx(Key k) const { return static_cast<std::size_t>(k);}
    };

    // std::unique_ptr<ddknd::input::IInputBackend> 
    // CreateGlfwInputBackend(const ddknd::window::Window& w);
} // namespace ddknd::input