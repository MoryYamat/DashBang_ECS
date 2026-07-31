#include <ddknd/fsm/axis_batch_compile_result.h>

namespace ddknd::fsm
{
    bool AxisBatchCompileResults::Succeeded() const
    {
        return diagnostics.empty();
    }

    void AxisBatchCompileResults::PrintDiagnostics() const
    {
        for(const auto d : diagnostics)
        {
            d.PrintMessage();
        }
    }
}