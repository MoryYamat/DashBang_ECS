#pragma once


// fwd
namespace ddknd::component
{
  struct TransformComponent;
  struct DebugCameraControllerComponent;
}
// fwd
namespace ddknd::input
{
  class DeviceInput;
}

namespace ddknd::debug
{
  // 
  class DebugCameraController
  {
    private:
      using Input = ::ddknd::input::DeviceInput;
      using DebugCameraControllerComponent = ::ddknd::component::DebugCameraControllerComponent;
      using TrasnformComponent = ::ddknd::component::TransformComponent;
    public:
      DebugCameraController(const Input& input, DebugCameraControllerComponent& debug, TrasnformComponent& tsr): input_(input), debug_(debug), transform_(tsr) {}

      void Update(float dt);

    private:
      const Input& input_;
      DebugCameraControllerComponent& debug_;
      TrasnformComponent& transform_;
  };
} // namespace ddknd::camera