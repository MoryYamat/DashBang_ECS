#include "ddknd/input/input.h"

#include <algorithm>
#include <cstddef>

namespace ddknd::input
{
    void ActionInputSystem::Update(const DeviceInput& input)
    {
        const auto action_count = mappings_.GetActionCount();

        if (actions_.size() != action_count)
        {
            actions_.resize(action_count);
        }

        if (action_values_.size() != action_count)
        {
            action_values_.resize(action_count);
        }

        std::fill(action_values_.begin(), action_values_.end(), 0.0f);

        auto setActionValue = [&](id_type actionID, float value)
        {
            if (actionID == InvalidID)
            {
                return;
            }
            const auto idx = static_cast<std::size_t>(actionID);
            if (idx >= action_values_.size())
            {
                return;
            }

            action_values_[idx] = value;
        };

        // key -> Action value
        for (std::size_t i = 0; i < input.KeyCount(); i++)
        {
            const auto key = static_cast<Key>(i);
            if (!input.IsKeyDown(key))
            {
                continue;
            }
            
            setActionValue(mappings_.GetActionFromKey(key), 1.0f);
        }

        // MouseAxis -> Action value
        const auto& mouse = input.Mouse();
        setActionValue(mappings_.GetActionFromMouseAxis(MouseAxis::DeltaX), static_cast<float>(mouse.deltaX));
        setActionValue(mappings_.GetActionFromMouseAxis(MouseAxis::DeltaY), static_cast<float>(mouse.deltaY));
        setActionValue(mappings_.GetActionFromMouseAxis(MouseAxis::WheelY), static_cast<float>(mouse.wheelY));

        // MouseButton -> Action value
        for (std::size_t i = 0; i < input.MouseButtonCount(); i++)
        {
            const auto button = static_cast<MouseButton>(i);
            if (!input.IsMouseButtonDown(button))
            {
                continue;
            }

            setActionValue(mappings_.GetActionFromMouseButton(button), 1.0f);
        }

        for (std::size_t i = 0; i < actions_.size(); ++i)
        {
            actions_[i].Update(action_values_[i]);
        }
    }

} // namespace ddknd::input