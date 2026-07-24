#include <iostream>

#include <ddknd/fsm/builder.h>

#include <cassert>

int main()
{
    std::cout << "hello fsm\n";

    ddknd::fsm::AxisBuilder axisBuilder{};

    auto testFSM = axisBuilder.DeclareFSM("tests");
    auto testFirstState = axisBuilder.DeclareState("testFirst");
    auto testSecondState = axisBuilder.DeclareState("testFirst");
    auto testCondition = axisBuilder.DeclareCondition("testConditionFirst");
    auto testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    assert(axisBuilder.IsValidFSMID(testFSM));
    assert(axisBuilder.IsValidStateID(testFirstState));
    assert(axisBuilder.IsValidStateID(testFirstState));
    assert(axisBuilder.IsValidStateID(testSecondState));
    assert(axisBuilder.IsValidConditionID(testCondition));
    assert(axisBuilder.IsValidProfileID(testProfile));

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);

    auto testTransition = testFSMBuilder.DeclareTransition("TestTransition", testFirstState, testSecondState);
    testFSMBuilder.DefineTransitionCondition(testTransition, testProfile, ddknd::fsm::ConditionDeclaration{.op = ddknd::fsm::Operator::LessEqual, .leftValue = 1.0f}, 1);

    auto buildResult = std::move(axisBuilder).Build();

    return 0;
}