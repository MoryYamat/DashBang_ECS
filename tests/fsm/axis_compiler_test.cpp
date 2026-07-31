#include <ddknd/fsm/builder.h>
#include <ddknd/fsm/compiler.h>

#include <ddknd/fsm/axis_definition.h>

#include "internal/fsm/compiler_internal.h"

#include "support/test_check.h"

#include <cassert>
#include <cstdint>

void CompilingCorrectlyConstructedFSMShouldSucceed()
{
    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder builder{};

    const FSMID testFSM = builder.DeclareFSM("tests");
    const StateID testStateFirst = builder.DeclareState("testFirst");
    const StateID testStateSecond = builder.DeclareState("testSecond");
    const ParameterID testParameter = builder.DeclareParameter(ValueType::Float, "testParameter");
    const ProfileID testProfile = builder.DeclareProfile("testProfileFirst");

    TEST_CHECK(builder.IsValidFSMID(testFSM));
    TEST_CHECK(builder.IsValidStateID(testStateFirst));
    TEST_CHECK(builder.IsValidStateID(testStateSecond));
    TEST_CHECK(builder.IsValidParameterID(testParameter));
    TEST_CHECK(builder.IsValidProfileID(testProfile));

    auto fsmBuilder = builder.GetFSMBuilder(testFSM);
    const TransitionID testTransition = fsmBuilder.DeclareTransition("testTransition", testStateFirst, testStateSecond);

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

void ValidateConditionFailsWhenParameterTypeMismatch()
{
    using namespace ddknd::fsm;

    AxisDefinition axis{};
    axis.parameters.push_back(ParameterDefinition{.type = ValueType::Float});

    ConditionDefinition conditionDefinition =
        ComparisonConditionDefinition{.op = Operator::Less,
                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                      .right = ConstantOperandDefinition{.value = std::uint32_t{20}}};

    std::vector<CompiledDiagnostic> diagnostic{};
    const bool ok = ddknd::fsm::internal::validateCondition(axis, conditionDefinition, diagnostic);
    TEST_CHECK(!ok);
    TEST_CHECK(!diagnostic.empty());
}

void OperatorShouldNotBeNoneAtCompileTime()
{
    using namespace ddknd::fsm;

    ComparisonConditionDefinition greater{.op = Operator::Greater,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};

    ComparisonConditionDefinition greaterEqual{.op = Operator::GreaterEqual,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};

    
    ComparisonConditionDefinition less{.op = Operator::Less,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};

    ComparisonConditionDefinition lessEqual{.op = Operator::LessEqual,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};
    
    ComparisonConditionDefinition equal{.op = Operator::Equal,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};

    ComparisonConditionDefinition notEqual{.op = Operator::NotEqual,
                                                      .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
                                                      .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};


    TEST_CHECK(internal::resolveOperator(less) == CompiledOperator::Less);
    TEST_CHECK(internal::resolveOperator(lessEqual) == CompiledOperator::LessEqual);
    TEST_CHECK(internal::resolveOperator(greater) == CompiledOperator::Greater);
    TEST_CHECK(internal::resolveOperator(greaterEqual) == CompiledOperator::GreaterEqual);
    TEST_CHECK(internal::resolveOperator(equal) == CompiledOperator::Equal);
    TEST_CHECK(internal::resolveOperator(notEqual) == CompiledOperator::NotEqual);

    /**
    * NOTE:
    * This pass is currently considered unreachable and terminate via assert.
    */
    // ComparisonConditionDefinition none{.op = Operator::None,
    //                                                   .left = ParameterOperandDefinition{.parameter = ParameterID{0}},
    //                                                   .right = ParameterOperandDefinition{.parameter = ParameterID{1}}};
    // TEST_CHECK(internal::resolveOperator(none))
}

void ValueTypeShouldBeSameTypeNameDefinedInVariant()
{
    using namespace ddknd::fsm;

    ConstantOperandDeclaration intValue{.value = int{5}};
    ConstantOperandDeclaration floatValue{.value = float{5.0f}};
    ConstantOperandDeclaration uintValue{.value = std::uint32_t{5}};
    ConstantOperandDeclaration boolValue{.value = true};

    TEST_CHECK(internal::resolveCompiledValueType(intValue.value) == CompiledValueType::Int);
    TEST_CHECK(internal::resolveCompiledValueType(floatValue.value) == CompiledValueType::Float);
    TEST_CHECK(internal::resolveCompiledValueType(uintValue.value) == CompiledValueType::UInt32);
    TEST_CHECK(internal::resolveCompiledValueType(boolValue.value) == CompiledValueType::Bool);
}

void CompiledOperandShouldBeDifferentBetweenConstantAndParameter()
{
    using namespace ddknd::fsm;

    ConstantOperandDefinition constantOperand{.value = int{5}};
    RawValue expectedConstantRawValue{.i = 5};
    CompiledValue expectedConstantCompileValue{.type = CompiledValueType::Int, .raw = expectedConstantRawValue};

    TEST_CHECK(internal::makeCompiledOperand(constantOperand).isConstant == true);
    TEST_CHECK(internal::makeCompiledOperand(constantOperand).parameter == ParameterID::Invalid());
    TEST_CHECK(internal::makeCompiledOperand(constantOperand).constant.type == CompiledValueType::Int);
    TEST_CHECK(internal::makeCompiledOperand(constantOperand).constant.raw.i == 5);


    ParameterOperandDefinition parameterOperand{.parameter = ParameterID{0}};

    TEST_CHECK(internal::makeCompiledOperand(parameterOperand).isConstant == false);
    TEST_CHECK(internal::makeCompiledOperand(parameterOperand).parameter == ParameterID{0});

}

int main()
{
    CompilingCorrectlyConstructedFSMShouldSucceed();
    ValidateConditionFailsWhenParameterTypeMismatch();
    OperatorShouldNotBeNoneAtCompileTime();
    ValueTypeShouldBeSameTypeNameDefinedInVariant();
    CompiledOperandShouldBeDifferentBetweenConstantAndParameter();
    /**
     * TODO:
     * - Decide specification: self-loop, same transition but different condition,
     * - Add Validation implementationsand tests:
     *      - Operator and Operand consistency (Operator::Less left true need to be errored.)
     */

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}