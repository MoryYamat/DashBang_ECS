#include <ddknd/system/ui_system.h>

#include <ddknd/ui/shape.h>
#include <ddknd/ui/ui_context.h>

#include <iostream>

namespace ddknd::system
{
    void HitTest(ui::UIContext& uiContext, input::DeviceInput& deviceInput)
    {
        const auto& shapes = uiContext.GetShapes();
        const auto& mouse = deviceInput.Mouse();


        for(int i = 0; i < shapes.size(); i++)
        {
            // float xMin = shapes[i].position.x() - (shapes[i].width / 2);
            // float xMax = shapes[i].position.x() + (shapes[i].width / 2);
            // float yMin = shapes[i].position.y() - (shapes[i].height / 2);
            // float yMax = shapes[i].position.y() + (shapes[i].height / 2);

            float xMin = shapes[i].position.x();
            float xMax = shapes[i].position.x() + (shapes[i].width);
            float yMin = shapes[i].position.y();
            float yMax = shapes[i].position.y() + (shapes[i].height);

            if(mouse.x >= xMin && mouse.x <= xMax && mouse.y >= yMin && mouse.y <= yMax )
            {
                std::cout << "shape (x, y)=" << xMin << "~" << xMax << ", " << yMin << "~" << yMax << "\n";
                std::cout << "mouse (x, y)=" << mouse.x << ", " << mouse.y << "\n";
            }
        }

        return;
    }
}