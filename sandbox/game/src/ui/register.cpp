#include "game/ui/register.h"

#include <ddknd/ui/shape.h>

namespace app::ui
{
    void RegisterUIs(ddknd::ui::UIContext& ui)
    {
        using namespace ddknd::ui;

        UIRectButton testButton{};
        testButton.shape.position = {200.f, 200.f}; 
        testButton.shape.color = {1.0f, 0.0f, 0.0f, 1.0f};
        testButton.shape.width = 200;
        testButton.shape.height = 200;

        ui.RegisterShpae(testButton.shape);
    }
}