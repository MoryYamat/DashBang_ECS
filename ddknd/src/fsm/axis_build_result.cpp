#include <ddknd/fsm/axis_build_result.h>

#include <ddknd/fsm/compiler.h>

namespace ddknd::fsm
{
    AxisBatchCompileResults AxisBuildResult::CompileAllAxis()
    {
        AxisBatchCompileResults result;

        for (std::size_t i = 0; i < definitions_.size(); i++)
        {
            auto compileResult = AxisCompiler::Compile(definitions_[i]);
            if (compileResult.Succeeded())
            {
                result.registry.Set((AxisID{static_cast<std::uint32_t>(i)}), std::move(*compileResult.axis));
            }
            else
            {
                result.diagnostics.insert(result.diagnostics.end(), compileResult.diagnostics.begin(),
                                          compileResult.diagnostics.end());
            }
        }
        return result;
    }
} // namespace ddknd::fsm