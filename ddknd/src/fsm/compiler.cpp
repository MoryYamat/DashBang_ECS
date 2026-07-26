#include <ddknd/fsm/compiler.h>
#include <ddknd/fsm/fsm_id.h>

#include "internal/fsm/compiler_internal.h"

#include <utility>

namespace ddknd::fsm
{
    AxisCompileResult AxisCompiler::Compile(AxisDefinition& definition)
    {
        AxisCompileResult result{};

        if (!internal::validateAxis(definition, result.diagnostics))
        {
            return result;
        }

        for (const auto& fsm : definition.fsms)
        {
            if (!internal::validateFSM(definition, fsm, result.diagnostics))
            {
                return result;
            }
        }

        CompiledAxis axisResult{};
        for (auto& fsm : definition.fsms)
        {
            internal::sortFSMDefinition(fsm);
            axisResult.fsms.emplace_back(internal::compileFSM(definition, fsm));
        }

        result.axis = std::move(axisResult);

        return result;
    }

} // namespace ddknd::fsm