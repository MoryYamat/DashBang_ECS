#pragma once

#include <ddknd/input/input.h>
#include <ddknd/ui/ui_context.h>


namespace ddknd::system
{
    void HitTest(ui::UIContext& uiContext, input::DeviceInput& deviceInput);
}