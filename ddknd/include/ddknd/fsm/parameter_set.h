#pragma once

#include <ddknd/fsm/compile_result.h>
#include <cstddef>
#include <vector>

namespace ddknd::fsm
{
    class ParameterSet
    {
      public:
        explicit ParameterSet(const std::size_t size);

        void SetParameter(const ParameterID id, const RawValue& value);

        RawValue GetParameter(const ParameterID id) const;

        bool IsValidParameterID(const ParameterID id) const;
      private:
        std::size_t size_;
        std::vector<RawValue> parameters_;
    };
}