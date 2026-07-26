#pragma once

#include "support/test_check.h"

#include <ddknd/fsm/builder.h>

void TestDeclareAxis()
{

    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder axisBuilder{};

    const FSMID testFSM = axisBuilder.DeclareFSM("tests");
    const StateID testState = axisBuilder.DeclareState("testFirst");
    const ParameterID testParameter = axisBuilder.DeclareParameter(ValueType::Float, "testParameter");
    const ProfileID testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    TEST_CHECK(testFSM.IsValid());
    TEST_CHECK(testState.IsValid());
    TEST_CHECK(testParameter.IsValid());
    TEST_CHECK(testProfile.IsValid());

    TEST_CHECK(axisBuilder.IsValidFSMID(testFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(testState));
    TEST_CHECK(axisBuilder.IsValidParameterID(testParameter));
    TEST_CHECK(axisBuilder.IsValidProfileID(testProfile));


    const FSMID nonDeclaredFSM{};
    const StateID nonDeclaredState{};
    const ParameterID nonDeclaredParameter{};
    const ProfileID nonDeclaredProfile{};

    TEST_CHECK(!nonDeclaredFSM.IsValid());
    TEST_CHECK(!nonDeclaredState.IsValid());
    TEST_CHECK(!nonDeclaredParameter.IsValid());
    TEST_CHECK(!nonDeclaredProfile.IsValid());

    TEST_CHECK(!axisBuilder.IsValidFSMID(nonDeclaredFSM));
    TEST_CHECK(!axisBuilder.IsValidStateID(nonDeclaredState));
    TEST_CHECK(!axisBuilder.IsValidParameterID(nonDeclaredParameter));
    TEST_CHECK(!axisBuilder.IsValidProfileID(nonDeclaredProfile));


    const FSMID outOfRangeFSM{100};
    const StateID outOfRangeState{100};
    const ParameterID outOfRangeParameter{100};
    const ProfileID outOfRangeProfile{100};

    TEST_CHECK(outOfRangeFSM.IsValid());
    TEST_CHECK(outOfRangeState.IsValid());
    TEST_CHECK(outOfRangeParameter.IsValid());
    TEST_CHECK(outOfRangeProfile.IsValid());

    TEST_CHECK(!axisBuilder.IsValidFSMID(outOfRangeFSM));
    TEST_CHECK(!axisBuilder.IsValidStateID(outOfRangeState));
    TEST_CHECK(!axisBuilder.IsValidParameterID(outOfRangeParameter));
    TEST_CHECK(!axisBuilder.IsValidProfileID(outOfRangeProfile));
}

void TestDeclareSameSymbols()
{
    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder axisBuilder{};

    const FSMID firstFSM = axisBuilder.DeclareFSM("tests");
    const FSMID secondFSM = axisBuilder.DeclareFSM("tests");

    const StateID firstState = axisBuilder.DeclareState("idle");
    const StateID secondState = axisBuilder.DeclareState("idle");

    const ParameterID firstParameter = axisBuilder.DeclareParameter(ValueType::Float, "testParameter");
    const ParameterID secondParameter = axisBuilder.DeclareParameter(ValueType::Float, "testParameter");
    const ParameterID thirdParameter = axisBuilder.DeclareParameter(ValueType::Bool, "testParameter");


    const ProfileID firstProfile = axisBuilder.DeclareProfile("basic");
    const ProfileID secondProfile = axisBuilder.DeclareProfile("basic");

    /**
    * The IDs must match between same declaration.
    */
    TEST_CHECK(firstFSM == secondFSM);
    TEST_CHECK(firstState == secondState);
    TEST_CHECK(firstParameter == secondParameter);
    
    /**
    * The ParameterID is unique to the name.
    * It is registered with the type specified initially, 
    * and subsequent type declarations for the same name are ignored.
    */
    TEST_CHECK(thirdParameter == firstParameter);
    TEST_CHECK(thirdParameter == secondParameter);
    TEST_CHECK(firstProfile == secondProfile);

    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(firstState));
    TEST_CHECK(axisBuilder.IsValidParameterID(firstParameter));
    TEST_CHECK(axisBuilder.IsValidProfileID(firstProfile));

    const FSMID fsmId = axisBuilder.DeclareFSM("tests");
    const StateID first = axisBuilder.DeclareState("first");
    const StateID second = axisBuilder.DeclareState("second");

    auto fsmBuilder = axisBuilder.GetFSMBuilder(fsmId);

    const TransitionID firstTransition = 
        fsmBuilder.DeclareTransition("transition",first,second);
    const TransitionID secondTransition = 
        fsmBuilder.DeclareTransition("transition",first,second);

    TEST_CHECK(firstTransition == secondTransition);
    TEST_CHECK(firstTransition.IsValid());
}

void TestDeclareDifferentSymbols()
{
    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder axisBuilder{};

    const FSMID firstFSM = axisBuilder.DeclareFSM("firstFSM");
    const FSMID secondFSM = axisBuilder.DeclareFSM("secondFSM");

    const StateID firstState = axisBuilder.DeclareState("firstState");
    const StateID secondState = axisBuilder.DeclareState("secondState");

    const ParameterID firstParameter = axisBuilder.DeclareParameter(ValueType::Float, "firstParameter");
    const ParameterID secondParameter = axisBuilder.DeclareParameter(ValueType::Float, "secondParameter");

    const ProfileID firstProfile = axisBuilder.DeclareProfile("firstProfile");
    const ProfileID secondProfile = axisBuilder.DeclareProfile("secondProfile");

    /**
    * The IDs must be unique across different declarations.
    */
    TEST_CHECK(firstFSM != secondFSM);
    TEST_CHECK(firstState != secondState);
    TEST_CHECK(firstParameter != secondParameter);
    TEST_CHECK(firstProfile != secondProfile);

    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(firstState));
    TEST_CHECK(axisBuilder.IsValidStateID(secondState));
    TEST_CHECK(axisBuilder.IsValidParameterID(firstParameter));
    TEST_CHECK(axisBuilder.IsValidParameterID(secondParameter));
    TEST_CHECK(axisBuilder.IsValidStateID(secondState));
    TEST_CHECK(axisBuilder.IsValidProfileID(firstProfile));
    TEST_CHECK(axisBuilder.IsValidProfileID(secondProfile));

    
    const FSMID fsmId = axisBuilder.DeclareFSM("tests");
    const StateID first = axisBuilder.DeclareState("first");
    const StateID second = axisBuilder.DeclareState("second");
    const StateID third = axisBuilder.DeclareState("third");

    auto fsmBuilder = axisBuilder.GetFSMBuilder(fsmId);

    const TransitionID firstTransition = 
        fsmBuilder.DeclareTransition("firstTransition",first,second);
    const TransitionID secondTransition = 
        fsmBuilder.DeclareTransition("secondtransition",second,third);

    TEST_CHECK(firstTransition != secondTransition);
    TEST_CHECK(firstTransition.IsValid());
}


void TestFSMDefinition()
{
    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder axisBuilder{};

    const FSMID testFSM = axisBuilder.DeclareFSM("tests");
    const StateID testStateFirst = axisBuilder.DeclareState("testFirst");
    const StateID testStateSecond = axisBuilder.DeclareState("testSecond");
    const ProfileID testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);
    const TransitionID testTransition = testFSMBuilder.DeclareTransition("testTransition",
                                                                testStateFirst,
                                                                testStateSecond);

    TEST_CHECK(testTransition.IsValid());
    testFSMBuilder.DefineTransitionCondition(testTransition,
                                             testProfile,
                                             AlwaysTrueConditionDeclaration{}
                                             , 1);

}

int main()
{
    TestDeclareAxis();
    
    TestDeclareSameSymbols();
    TestDeclareDifferentSymbols();
    
    TestFSMDefinition();
    
    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}