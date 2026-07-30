#include <ddknd/fsm/axis_registry.h>

#include <ddknd/fsm/compile_result.h>

#include "support/test_check.h"

void InvalidAxisIDShouldBeInvalid()
{
    using namespace ddknd::fsm;

    CompiledAxis dummyAxis{};

    AxisRegistry axisRegistry{};

    AxisID validAxisID{0};
    axisRegistry.Set(validAxisID, std::move(dummyAxis));
    TEST_CHECK(axisRegistry.IsValidAxisID(validAxisID));

    AxisID InvalidAxisID{};
    TEST_CHECK(!axisRegistry.IsValidAxisID(InvalidAxisID));

    AxisID axisIDBeforeSetting{10};
    TEST_CHECK(!axisRegistry.IsValidAxisID(axisIDBeforeSetting));

    AxisID axisIDAfterSetting{20};
    CompiledAxis dummyAxisSecond{};
    axisRegistry.Set(axisIDAfterSetting, std::move(dummyAxisSecond));
    // TEST_CHECK(axisRegistry.IsValidAxisID(axisIDAfterSetting));
}

int main()
{

    InvalidAxisIDShouldBeInvalid();
    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}