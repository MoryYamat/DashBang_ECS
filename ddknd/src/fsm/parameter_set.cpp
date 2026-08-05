#include <ddknd/fsm/parameter_set.h>

#include <iostream>

namespace ddknd::fsm
{

    ParameterSet::ParameterSet(const std::size_t size) : size_(size)
    {
        parameters_.resize(size);
    }

    void ParameterSet::SetParameter(const ParameterID id, const RawValue& value)
    {
        if (!id.IsValid())
        {
            std::cerr << "Invalid ParameterID" << "\n";
            return;
        }

        auto index = static_cast<std::size_t>(id.Value());

        if (index >= parameters_.size())
        {
            std::cerr << "Invalid ParameterID =" << id.Value() << "\n";
            return;
        }
        parameters_[index] = value;
    }
    RawValue ParameterSet::GetParameter(const ParameterID id) const
    {
        if(!IsValidParameterID(id))
        {
            return {};
        }

        const auto index = static_cast<std::size_t>(id.Value());

        return RawValue{parameters_[index]};
    }

    bool ParameterSet::IsValidParameterID(const ParameterID id) const
    {
        if (!id.IsValid())
        {
            std::cerr << "Invalid ParameterID" << "\n";
            return false;
        }

        if (static_cast<std::size_t>(id.Value()) >= parameters_.size())
        {
            std::cerr << "Invalid ParameterID =" << id.Value() << "\n";
            return false;
        }
        return true;
    }
} // namespace ddknd::fsm