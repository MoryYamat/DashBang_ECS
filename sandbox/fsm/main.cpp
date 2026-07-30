#include <iostream>

#include <ddknd/fsm/builder.h>
#include <ddknd/fsm/compiler.h>

#include <ddknd/fsm/axis_registry.h>

#include <cassert>

int main()
{
    std::cout << "hello fsm\n";

    ddknd::fsm::AxisBuilder axisBuilder{};

    auto testFSM = axisBuilder.DeclareFSM("tests");
    auto testFirstState = axisBuilder.DeclareState("testFirst");
    auto testSecondState = axisBuilder.DeclareState("testSecond");
    auto testThirdState = axisBuilder.DeclareState("testThird");
    auto testParameter = axisBuilder.DeclareParameter(ddknd::fsm::ValueType::Float, "testParameter");
    auto testProfile = axisBuilder.DeclareProfile("testProfileFirst");
    auto testProfileSecond = axisBuilder.DeclareProfile("testProfileSecond");

    assert(axisBuilder.IsValidFSMID(testFSM));
    assert(axisBuilder.IsValidStateID(testFirstState));
    assert(axisBuilder.IsValidStateID(testSecondState));
    assert(axisBuilder.IsValidParameterID(testParameter));
    assert(axisBuilder.IsValidProfileID(testProfile));

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);

    auto testTransition = testFSMBuilder.DeclareTransition("TestTransition", testFirstState, testSecondState);
    auto testTransitionSecond =
        testFSMBuilder.DeclareTransition("TestTransitionSecond", testSecondState, testThirdState);
    auto testTransitionThird = testFSMBuilder.DeclareTransition("TestTransitionThird", testSecondState, testFirstState);

    // Transition = 0
    testFSMBuilder.DefineTransitionCondition(
        testTransition, testProfile,
        ddknd::fsm::ComparisonConditionDeclaration{
            .op = ddknd::fsm::Operator::LessEqual,
            .left = ddknd::fsm::ConstantOperandDeclaration{.value = 1.0f},
            .right = ddknd::fsm::ParameterOperandDeclaration{.parameter = testParameter}},
        1);

    // Transition = 1, Profile = 0, priority = 1
    testFSMBuilder.DefineTransitionCondition(
        testTransitionSecond, testProfile,
        ddknd::fsm::ComparisonConditionDeclaration{
            .op = ddknd::fsm::Operator::LessEqual,
            .left = ddknd::fsm::ConstantOperandDeclaration{.value = 1.0f},
            .right = ddknd::fsm::ParameterOperandDeclaration{.parameter = testParameter}},
        1);

    // Transition = 1, Profile = 1, priority = 1
    testFSMBuilder.DefineTransitionCondition(
        testTransitionSecond, testProfileSecond,
        ddknd::fsm::ComparisonConditionDeclaration{
            .op = ddknd::fsm::Operator::LessEqual,
            .left = ddknd::fsm::ConstantOperandDeclaration{.value = 1.0f},
            .right = ddknd::fsm::ParameterOperandDeclaration{.parameter = testParameter}},
        1);

    testFSMBuilder.DefineTransitionCondition(
        testTransitionThird, testProfile,
        ddknd::fsm::ComparisonConditionDeclaration{
            .op = ddknd::fsm::Operator::LessEqual,
            .left = ddknd::fsm::ConstantOperandDeclaration{.value = 1.0f},
            .right = ddknd::fsm::ParameterOperandDeclaration{.parameter = testParameter}},
        1);

    auto buildResult = std::move(axisBuilder).Build();

    auto compiledResult = ddknd::fsm::AxisCompiler::Compile(buildResult);

    if (!compiledResult.Succeeded())
    {
        for (const auto& m : compiledResult.diagnostics)
        {
            std::cerr << m.message << "\n";
        }
    }

    ddknd::fsm::AxisRegistry registry{};

    ddknd::fsm::AxisID testAxis{0};
    registry.Set(testAxis, std::move(*compiledResult.axis));
    assert(registry.IsValidAxisID(testAxis));
    auto& GotAxis = registry.Get(testAxis);
    
    ddknd::fsm::AxisID testAxisSecond{100};
    assert(!registry.IsValidAxisID(testAxisSecond));

    return 0;
}