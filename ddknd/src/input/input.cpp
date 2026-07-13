#include "ddknd/input/input.h"

#include <cstddef>
#include <vector>


namespace ddknd::input
{
    void ActionInputSystem::Update(const DeviceInput& input)
    {
        const auto action_count = mappings_.GetActionCount();

        if (actions_.size() != action_count)
            actions_.resize(action_count);

        if (action_values_.size() != action_count)
            action_values_.resize(action_count);

        std::fill(action_values_.begin(), action_values_.end(), 0.0f);

        auto setActionValue = [&](id_type actionID, float value)
        {
            if (actionID == InvalidID)
                return;
            const auto idx = static_cast<std::size_t>(actionID);
            if (idx >= action_values_.size())
                return;
            action_values_[idx] = value;
        };

        // =============================
        //  key -> Action value
        // =============================
        for (std::size_t i = 0; i < input.KeyCount(); i++)
        {
            auto key = static_cast<Key>(i);

            auto actionID = mappings_.GetActionFromKey(key);
            if (actionID == InvalidID)
                continue;

            if (static_cast<std::size_t>(actionID) >= action_values_.size())
                continue;
            if (input.IsKeyDown(key))
            {
                action_values_[static_cast<std::size_t>(actionID)] = 1.0f;
            }
        }

        // =============================
        //  MouseAxis -> Action value
        // =============================
        const auto& mouse = input.Mouse();
        {
            auto actionID = mappings_.GetActionFromMouseAxis(MouseAxis::DeltaX);
            setActionValue(actionID, static_cast<float>(mouse.deltaX));
        }
        {
            auto actionID = mappings_.GetActionFromMouseAxis(MouseAxis::DeltaY);
            setActionValue(actionID, static_cast<float>(mouse.deltaY));
        }
        {
            auto actionID = mappings_.GetActionFromMouseAxis(MouseAxis::WheelY);
            setActionValue(actionID, static_cast<float>(mouse.wheelY));
        }

        // =============================
        //  MouseButton -> Action value
        // =============================
        for (std::size_t i = 0; i < input.MouseButtonCount(); i++)
        {
            auto button = static_cast<MouseButton>(i);

            auto actionID = mappings_.GetActionFromMouseButton(button);
            if (actionID == InvalidID)
            {
                continue;
            }
                
            if (static_cast<std::size_t>(actionID) >= action_values_.size())
                continue;
            if (input.IsMouseButtonDown(button))
            {
                action_values_[static_cast<std::size_t>(actionID)] = 1.0f;
            }
        }

        // =============================
        // Final ActionState update
        // =============================
        for (std::size_t i = 0; i < actions_.size(); ++i)
        {
            actions_[i].Update(action_values_[i]);
        }
    }

} // namespace ddknd::input