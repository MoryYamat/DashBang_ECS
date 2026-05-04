#pragma once

#include <memory>

namespace ddknd::camera
{
    class CameraController
    {
      public:
        void Update();
      private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
} // namespace ddknd::camera