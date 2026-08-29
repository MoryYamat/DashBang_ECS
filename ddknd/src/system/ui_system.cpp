#include <ddknd/system/ui_system.h>

#include <ddknd/ui/shape.h>
#include <ddknd/ui/ui_context.h>
#include <ddknd/input/input.h>

#include <iostream>


namespace ddknd::system
{
    void HitTest(ui::UIContext& uiContext, input::DeviceInput& deviceInput)
    {
        // const auto& shapes = uiContext.GetShapes();
        auto& buttons = uiContext.GetUIRectButtons();
        const auto& mouse = deviceInput.Mouse();


        for(int i = 0; i < buttons.size(); i++)
        {
            // float xMin = shapes[i].position.x() - (shapes[i].width / 2);
            // float xMax = shapes[i].position.x() + (shapes[i].width / 2);
            // float yMin = shapes[i].position.y() - (shapes[i].height / 2);
            // float yMax = shapes[i].position.y() + (shapes[i].height / 2);

            float xMin = buttons[i].shape.position.x();
            float xMax = buttons[i].shape.position.x() + (buttons[i].shape.width);
            float yMin = buttons[i].shape.position.y();
            float yMax = buttons[i].shape.position.y() + (buttons[i].shape.height);

            if(mouse.x >= xMin && mouse.x <= xMax && mouse.y >= yMin && mouse.y <= yMax )
            {
                buttons[i].hovered = true;
                if(deviceInput.IsMouseButtonPressed(ddknd::input::MouseButton::LEFT_CLICK)){
                    buttons[i].pressed = true;
                }
            }else{
                buttons[i].hovered = false;
            }
        }

        return;
    }

    void UpdateUIEvent(const ui::UIContext& uiContext){
        const auto buttons = uiContext.GetUIRectButtons();

        for(int i = 0; i < buttons.size(); i++){
            if(buttons[i].pressed){
                std::cout << "pressed\n";
            }
        }
    }

    void ResetUIContext(ui::UIContext& uiContext)
    {
        uiContext.ResetUIEvenet();
    }
}