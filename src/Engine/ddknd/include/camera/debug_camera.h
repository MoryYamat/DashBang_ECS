#pragma once


// fwd
namespace ddknd::component
{
  struct DebugCameraComponent;
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
      using DebugCameraComponent = ::ddknd::component::DebugCameraComponent;
    public:
      DebugCameraController(const Input& input, DebugCameraComponent& cam): input_(input), cam_(cam) {}

      void Update();

    private:
      const Input& input_;
      DebugCameraComponent& cam_;
      
  };
} // namespace ddknd::camera