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
        if(!id.IsValid())
        {
            std::cerr << "Invalid ParameterID" << "\n";
            return;
        }

        auto index = static_cast<std::size_t>(id.Value());

        if(index >= parameters_.size() )
        {
            std::cerr << "Invalid ParameterID =" << id.Value() << "\n";
            return;
        }
        parameters_[index] = value;
    }
}