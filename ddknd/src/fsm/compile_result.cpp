#include <ddknd/fsm/compile_result.h>

#include <cassert>
#include <cstddef>

namespace ddknd::fsm
{
    const CompiledFSM& CompiledAxis::GetFSM(FSMID id) const
    {
        assert(id.IsValid());
        auto index = static_cast<std::size_t>(id.Value());
        assert( index <fsms.size());

        return fsms[index];
    }
}