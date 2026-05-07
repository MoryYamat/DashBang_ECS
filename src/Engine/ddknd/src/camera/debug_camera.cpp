#include "camera/debug_camera.h"

#include "component/test_component.h"
#include "input/input.h"

namespace ddknd::debug
{
    void DebugCameraController::Update()
    {
        using Key = ::ddknd::input::Key;

        auto& pos = cam_.pos;

        //@TODO: need to consider direction cotroll (privided by mouse)

        if(input_.isPressing(Key::W))
        {
            pos[2] -= 1.0f;
        }
        if(input_.isPressing(Key::A))
        {
            pos[0] -= 1.0f;
        }
        if(input_.isPressing(Key::S))
        {
            pos[2] += 1.0f;
        }
        if(input_.isPressing(Key::D))
        {
            pos[0] += 1.0f;
        }
    }
}