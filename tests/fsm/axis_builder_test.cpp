#pragma once

#include "support/test_check.h"

#include <ddknd/fsm/builder.h>

void TestDeclareAxis()
{

    using namespace ddknd::fsm;

    ddknd::fsm::AxisBuilder axisBuilder{};

    const FSMID testFSM = axisBuilder.DeclareFSM("tests");
    const StateID testState = axisBuilder.DeclareState("testFirst");
    const ConditionID testCondition = axisBuilder.DeclareCondition("testConditionFirst");
    const ProfileID testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    TEST_CHECK(testFSM.IsValid());
    TEST_CHECK(testState.IsValid());
    TEST_CHECK(testCondition.IsValid());
    TEST_CHECK(testProfile.IsValid());

    TEST_CHECK(axisBuilder.IsValidFSMID(testFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(testState));
    TEST_CHECK(axisBuilder.IsValidConditionID(testCondition));
    TEST_CHECK(axisBuilder.IsValidProfileID(testProfile));


    const FSMID nonDeclaredFSM{};
    const StateID nonDeclaredState{};
    const ConditionID nonDeclaredCondition{};
    const ProfileID nonDeclaredProfile{};

    TEST_CHECK(!nonDeclaredFSM.IsValid());
    TEST_CHECK(!nonDeclaredState.IsValid());
    TEST_CHECK(!nonDeclaredCondition.IsValid());
    TEST_CHECK(!nonDeclaredProfile.IsValid());

    TEST_CHECK(!axisBuilder.IsValidFSMID(nonDeclaredFSM));
    TEST_CHECK(!axisBuilder.IsValidStateID(nonDeclaredState));
    TEST_CHECK(!axisBuilder.IsValidConditionID(nonDeclaredCondition));
    TEST_CHECK(!axisBuilder.IsValidProfileID(nonDeclaredProfile));


    const FSMID outOfRangeFSM{100};
    const StateID outOfRangeState{100};
    const ConditionID outOfRangeCondition{100};
    const ProfileID outOfRangeProfile{100};

    TEST_CHECK(outOfRangeFSM.IsValid());
    TEST_CHECK(outOfRangeState.IsValid());
    TEST_CHECK(outOfRangeCondition.IsValid());
    TEST_CHECK(outOfRangeProfile.IsValid());

    TEST_CHECK(!axisBuilder.IsValidFSMID(outOfRangeFSM));
    TEST_CHECK(!axisBuilder.IsValidStateID(outOfRangeState));
    TEST_CHECK(!axisBuilder.IsValidConditionID(outOfRangeCondition));
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

    const ConditionID firstCondition = axisBuilder.DeclareCondition("input");
    const ConditionID secondCondition = axisBuilder.DeclareCondition("input");

    const ProfileID firstProfile = axisBuilder.DeclareProfile("basic");
    const ProfileID secondProfile = axisBuilder.DeclareProfile("basic");

    /**
    * The IDs must match between same declaration.
    */
    TEST_CHECK(firstFSM == secondFSM);
    TEST_CHECK(firstState == secondState);
    TEST_CHECK(firstCondition == secondCondition);
    TEST_CHECK(firstProfile == secondProfile);

    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(firstState));
    TEST_CHECK(axisBuilder.IsValidConditionID(firstCondition));
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

    const ConditionID firstCondition = axisBuilder.DeclareCondition("firstCondition");
    const ConditionID secondCondition = axisBuilder.DeclareCondition("secondCondition");

    const ProfileID firstProfile = axisBuilder.DeclareProfile("firstProfile");
    const ProfileID secondProfile = axisBuilder.DeclareProfile("secondProfile");

    /**
    * The IDs must be unique across different declarations.
    */
    TEST_CHECK(firstFSM != secondFSM);
    TEST_CHECK(firstState != secondState);
    TEST_CHECK(firstCondition != secondCondition);
    TEST_CHECK(firstProfile != secondProfile);

    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidFSMID(firstFSM));
    TEST_CHECK(axisBuilder.IsValidStateID(firstState));
    TEST_CHECK(axisBuilder.IsValidStateID(secondState));
    TEST_CHECK(axisBuilder.IsValidConditionID(firstCondition));
    TEST_CHECK(axisBuilder.IsValidConditionID(secondCondition));
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
    const ConditionID testCondition = axisBuilder.DeclareCondition("testConditionFirst");
    const ProfileID testProfile = axisBuilder.DeclareProfile("testProfileFirst");

    auto testFSMBuilder = axisBuilder.GetFSMBuilder(testFSM);
    const TransitionID testTransition = testFSMBuilder.DeclareTransition("testTransition",
                                                                testStateFirst,
                                                                testStateSecond);

    TEST_CHECK(testTransition.IsValid());
    testFSMBuilder.DefineTransitionCondition(testTransition,
                                             testProfile,
                                             ConditionDefinition{
                                                .op = Operator::AlwaysTrue
                                             }, 1);

}

int main()
{
    TestDeclareAxis();
    
    TestDeclareSameSymbols();
    TestDeclareDifferentSymbols();
    
    TestFSMDefinition();
    
    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}