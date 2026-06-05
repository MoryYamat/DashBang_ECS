#include "ddknd/input/input.h"

#include <vector>
#include <cstddef>

namespace ddknd::input
{
    void ActionInputSystem::Update(const DeviceInput& input)
    {
        const auto action_count = mappings_.GetActionCount();

        if(actions_.size() != action_count)
            actions_.resize(action_count);

        if(action_values_.size() != action_count)
            action_values_.resize(action_count);

        std::fill(action_values_.begin(), action_values_.end(), 0.0f);

        for(std::size_t i = 0; i < input.KeyCount(); i++)
        {
            auto key = static_cast<Key>(i);

            auto actionID = mappings_.GetActionFromKey(key);
            if(actionID == InvalidID)
                continue;

            if(static_cast<std::size_t>(actionID) >= action_values_.size())
                continue;
            if(input.isPressing(key))
            {
                action_values_[static_cast<std::size_t>(actionID)] = 1.0f;
            }
        }

        for(std::size_t i = 0; i < actions_.size(); ++i)
        {
            actions_[i].Update(action_values_[i]);
        }
    }

} // namespace ddknd::input