#include <ddknd/fsm/builder.h>
#include <ddknd/fsm/compiler.h>

#include "support/test_check.h"

#include <cassert>

void CompilationSucceseeded()
{
    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder builder{};

    const FSMID testFSM = builder.DeclareFSM("tests");
    const StateID testStateFirst = builder.DeclareState("testFirst");
    const StateID testStateSecond = builder.DeclareState("testSecond");
    const ParameterID testParameter = builder.DeclaraParameter("testParameter");
    const ProfileID testProfile = builder.DeclareProfile("testProfileFirst");

    TEST_CHECK(builder.IsValidFSMID(testFSM));
    TEST_CHECK(builder.IsValidStateID(testStateFirst));
    TEST_CHECK(builder.IsValidStateID(testStateSecond));
    TEST_CHECK(builder.IsValidParameterID(testParameter));
    TEST_CHECK(builder.IsValidProfileID(testProfile));

    auto fsmBuilder = builder.GetFSMBuilder(testFSM);
    const TransitionID testTransition =
        fsmBuilder.DeclareTransition("testTransition", testStateFirst, testStateSecond);

    fsmBuilder.DefineTransitionCondition(
        testTransition, testProfile,
        ComparisonConditionDeclaration{.op = Operator::Less,
                                       .left = ParameterOperandDeclaration{.parameter = testParameter},
                                       .right = ConstantOperandDeclaration{.value = 10.0f}},
        1);
    TEST_CHECK(testTransition.IsValid());


    auto builtResult = std::move(builder).Build();
    auto compiledResult = AxisCompiler::Compile(builtResult);

    TEST_CHECK(compiledResult.Succeeded());
}

int main()
{
    CompilationSucceseeded();

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}