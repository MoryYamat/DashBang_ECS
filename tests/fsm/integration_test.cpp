// ====
#include <ddknd/fsm/axis_builder_registry.h>
#include <ddknd/fsm/fsm_id.h>

// ====
#include "support/test_check.h"

/**
* N is NodeCount, TransitionCount is total count of bidirectional transitions.
* TransitionCount = N(N-1) 
*/
struct TestMovementAxis
{
    ddknd::fsm::AxisID axis;
    ddknd::fsm::FSMID fsm;

    ddknd::fsm::StateID idle;
    ddknd::fsm::StateID run;
    ddknd::fsm::StateID fly;

    ddknd::fsm::ParameterID movementRequestInput;
    ddknd::fsm::ParameterID flyRequestInput;
    ddknd::fsm::ParameterID movementSpeed;
    ddknd::fsm::ParameterID flyState;

    ddknd::fsm::ProfileID basicProfile;
    ddknd::fsm::ProfileID flyingProfile;

    ddknd::fsm::TransitionID idleToRun;
    ddknd::fsm::TransitionID idleToFly;
    ddknd::fsm::TransitionID runToIdle;
    ddknd::fsm::TransitionID flyToIdle;
    ddknd::fsm::TransitionID runToFly;
    ddknd::fsm::TransitionID flyToRun;
};

struct TestSkillAxis
{
    ddknd::fsm::AxisID axis;
    ddknd::fsm::FSMID fsm;

    ddknd::fsm::StateID none;
    ddknd::fsm::StateID cast;
    ddknd::fsm::StateID active;
    ddknd::fsm::StateID recover;

    ddknd::fsm::ParameterID triggerInput;
    ddknd::fsm::ParameterID castToActiveTime;
    ddknd::fsm::ParameterID activeToRecoverTime;
    ddknd::fsm::ParameterID recoverToNoneTime;

    ddknd::fsm::ProfileID testFirstProfile;
    ddknd::fsm::ProfileID testSecondProfile;

    ddknd::fsm::TransitionID noneToCast;
    ddknd::fsm::TransitionID castToActive;
    ddknd::fsm::TransitionID activeToRecover;
    ddknd::fsm::TransitionID recoverToNone;
};

void ConstructedAndExecuteTransitionCorrectly()
{
    using namespace ddknd::fsm;

    AxisBuilderRegistry axisRegistry;

    TestMovementAxis movementAxis;

    auto movementAxisBuildHandle = axisRegistry.GetOrCreateAxis("MovementAxis");
    auto& movementAxisBuilder = movementAxisBuildHandle.builder;

    movementAxis.axis = movementAxisBuildHandle.id;

    movementAxis.fsm = movementAxisBuilder.DeclareFSM("MovementBasicFSM");

    movementAxis.idle = movementAxisBuilder.DeclareState("Idle");
    movementAxis.run = movementAxisBuilder.DeclareState("Run");
    movementAxis.fly = movementAxisBuilder.DeclareState("Fly");
    movementAxis.movementRequestInput = movementAxisBuilder.DeclareParameter(ValueType::FVec2, "MovementRequestInput");
    movementAxis.movementSpeed = movementAxisBuilder.DeclareParameter(ValueType::FVec2, "MovementSpeed");

    movementAxis.basicProfile = movementAxisBuilder.DeclareProfile("BasicProfile");

    auto movementFSMBuilder = movementAxisBuilder.GetFSMBuilder(movementAxis.fsm);
    movementAxis.idleToRun = movementFSMBuilder.DeclareTransition("IdleToRun", movementAxis.idle, movementAxis.run);
    movementAxis.runToIdle = movementFSMBuilder.DeclareTransition("RunToIdle", movementAxis.run, movementAxis.idle);
    movementAxis.idleToFly = movementFSMBuilder.DeclareTransition("IdleToFly", movementAxis.idle, movementAxis.fly);
    movementAxis.runToFly = movementFSMBuilder.DeclareTransition("RunToFly", movementAxis.run, movementAxis.fly);
    movementAxis.flyToIdle = movementFSMBuilder.DeclareTransition("FlyToIdle", movementAxis.fly, movementAxis.idle);
    movementAxis.flyToRun = movementFSMBuilder.DeclareTransition("FlyToRun", movementAxis.fly, movementAxis.run);

    movementFSMBuilder.DefineTransitionCondition(
        movementAxis.idleToRun, movementAxis.basicProfile,
        ConditionDeclaration{ComparisonConditionDeclaration{
            .op = Operator::Less,
            .left = ParameterOperandDeclaration{.parameter = movementAxis.movementRequestInput},
            .right = ConstantOperandDeclaration{.value = 0.0f}}},
        10);
    movementFSMBuilder.DefineTransitionCondition(
        movementAxis.runToIdle, movementAxis.basicProfile,
        ConditionDeclaration{
            ComparisonConditionDeclaration{.op = Operator::LessEqual,
                                           .left = ParameterOperandDeclaration{.parameter = movementAxis.movementSpeed},
                                           .right = ConstantOperandDeclaration{.value = 0.0f}}},
        10);

    
}

int main()
{

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}