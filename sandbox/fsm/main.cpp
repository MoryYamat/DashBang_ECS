#include <iostream>

#include <ddknd/fsm/builder.h>

int main()
{
    std::cout << "hello fsm\n";

    ddknd::fsm::AxisBuilder axisBuilder{};

    auto testFSM = axisBuilder.DeclareFSM("tests");
    auto testState = axisBuilder.DeclareState("testFirst");
    auto testCondition = axisBuilder.DeclareCondition("testConditionFirst");
    auto testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    std::cout << "fsm=" << testFSM.Value() << "\n";
    std::cout << "state=" << testState.Value() << "\n";
    std::cout << "condition=" << testCondition.Value() << "\n";
    std::cout << "profile=" << testProfile.Value() << "\n";

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);

    

    return 0;
}