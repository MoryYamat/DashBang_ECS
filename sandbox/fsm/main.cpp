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
    auto testParameter = axisBuilder.DeclaraParameter("testParameter");
    auto testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    assert(axisBuilder.IsValidFSMID(testFSM));
    assert(axisBuilder.IsValidStateID(testFirstState));
    assert(axisBuilder.IsValidStateID(testSecondState));
    assert(axisBuilder.IsValidParameterID(testParameter));
    assert(axisBuilder.IsValidProfileID(testProfile));

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);

    auto testTransition = testFSMBuilder.DeclareTransition("TestTransition", testFirstState, testSecondState);
    testFSMBuilder.DefineTransitionCondition(
        testTransition,
        testProfile,
        ddknd::fsm::ComparisonConditionDeclaration{.op = ddknd::fsm::Operator::LessEqual,
                                         .left = ddknd::fsm::ConstantOperandDeclaration{.value = 1.0f},
                                         .right =ddknd::fsm::ParameterOperandDeclaration{.parameter = testParameter}},
        1);

    auto buildResult = std::move(axisBuilder).Build();

    return 0;
}