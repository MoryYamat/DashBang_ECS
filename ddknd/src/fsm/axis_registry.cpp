#include <ddknd/fsm/axis_registry.h>

#include <cassert>
#include <iostream>

namespace ddknd::fsm
{
    void AxisRegistry::Set(const AxisID id, CompiledAxis&& axis)
    {
        if(!id.IsValid())
        {
            std::cerr << "[AxisRegistry::Set] Invalid Axis ID\n";
            return;
        }

        const auto index = static_cast<std::size_t>(id.Value());

        if(index >= axes_.size())
        {
            axes_.resize(index+1);
            axes_[index] = std::move(axis);
            return;
        }
        else
        {
            axes_[index] = std::move(axis);
            return;
        }
    }

    bool AxisRegistry::IsValidAxisID(AxisID id) const
    {
        if(!id.IsValid())
        {
            return false;
        }

        if(static_cast<std::size_t>(id.Value()) >= axes_.size())
        {
            return false;
        }

        return true;
    }

    /**
    * @note
    * ID validation is required before calling this method.
    * Precondition: Contains(id) == true
    */
    const CompiledAxis& AxisRegistry::Get(AxisID id) const
    {
        assert(IsValidAxisID(id));

        return axes_[static_cast<std::size_t>(id.Value())];
    }
}

