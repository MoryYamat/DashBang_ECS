from ddknd_fsm.dsl import *
from ddknd_fsm.authoring import *
from ddknd_fsm.validation import *
import pytest

@pytest.fixture
def basic_fsm():
    fsm = FSM(name="Test")
    fsm.state("A", False)
    fsm.parameter("P", float)

    return fsm

# individual rule test


def test_state_type_non_StateDef_is_Invalid(basic_fsm):
    basic_fsm._definition.states.append(State(StateDef("B", False)))
    ctx = ValidationContext(basic_fsm._definition)

    validate_state_type(ctx)

    assert ctx.errors[0].code == "Invalid state Type"
    assert ctx.errors[0].message == "State type must be StateDef."
    assert len(ctx.errors) == 1


def test_state_defined_at_different_fsm_is_invalid(basic_fsm):
    external_fsm = FSM("ExternalFSM")
    external_state = StateDef(name="External", owner=external_fsm, initial=False)

    ctx = ValidationContext(basic_fsm._definition)

    ctx.valid_states.append(external_state)

    validate_state_referential(ctx)

    assert ctx.errors[0].code == "Invalid fsm reference"
    assert len(ctx.errors) == 1

def test_state_name_duplication_is_invalid(basic_fsm):
    dup_state = basic_fsm.state("A")

    ctx = ValidationContext(basic_fsm._definition)

    ctx.valid_states.append(basic_fsm.states[0])
    ctx.valid_states.append(dup_state._definition)

    validate_state_unique_names(ctx)

    assert ctx.errors[0].code == "State name duplication error"
    assert len(ctx.errors) == 1

def test_there_must_be_exactly_one_initial_state(basic_fsm):
    ctx_none_initial_state = ValidationContext(basic_fsm._definition)

    ctx_none_initial_state.valid_states.append(basic_fsm.states[0])

    validate_initial_state(ctx_none_initial_state)

    assert ctx_none_initial_state.errors[0].code == "Initial state count error"
    assert ctx_none_initial_state.errors[0].message == "At least one initial state is required."
    assert len(ctx_none_initial_state.errors) == 1

    basic_fsm.states[0].initial = True
    secondary_initial_state = basic_fsm.state("SecondaryInitialState", True)

    ctx_dobule_initial_state = ValidationContext(basic_fsm)
    ctx_dobule_initial_state.valid_states.append(basic_fsm.states[0])
    ctx_dobule_initial_state.valid_states.append(secondary_initial_state._definition)

    validate_initial_state(ctx_dobule_initial_state)

    assert ctx_dobule_initial_state.errors[0].code == "Initial state count error"
    assert ctx_dobule_initial_state.errors[0].message == "There are two or more initial states."
    assert len(ctx_dobule_initial_state.errors) == 1

def test_parameter_type_non_ParameterDef_is_Invalid(basic_fsm):
    basic_fsm._definition.parameters.append(Parameter(ParameterDef("Invalid", basic_fsm, bool)))

    ctx = ValidationContext(basic_fsm._definition)

    validate_parameter_types(ctx)

    assert ctx.errors[0].code == "Parameter type error"
    assert ctx.errors[0].message == "Invalid parameter type."
    assert len(ctx.errors) == 1

def test_parameter_type_non_defined_value_type_is_invalid(basic_fsm):
    @dataclass
    class MyType:
        pass
    basic_fsm._definition.parameters.append(ParameterDef("Invalid", basic_fsm, MyType))

    ctx = ValidationContext(basic_fsm._definition)

    validate_parameter_types(ctx)

    assert ctx.errors[0].code == "Parameter type error"
    assert ctx.errors[0].message == "Invalid parameter type."
    assert len(ctx.errors) == 1

def test_parameter_name_duplication_is_invalid(basic_fsm):
    dup_p =  basic_fsm.parameter("P", bool)

    ctx = ValidationContext(basic_fsm._definition)

    ctx.valid_parameters.append(basic_fsm._definition.parameters[0])
    ctx.valid_parameters.append(dup_p)

    validate_parameter_names(ctx)

    assert ctx.errors[0].code == "Parameter name duplication error"
    assert ctx.errors[0].message == "There is a duplicate parameter name."
    assert len(ctx.errors) == 1

def test_transition_type_non_TransitionDef_is_Invalid(basic_fsm):
    basic_fsm.transitions.append("Invalid")

    ctx = ValidationContext(basic_fsm._definition)

    validate_transition_types(ctx)

    assert ctx.errors[0].code == "Transition type error"
    assert ctx.errors[0].message == "Invalid Transition Type"
    assert len(ctx.errors) == 1


def test_transition_state_type_non_StateDef_is_Invalid(basic_fsm):
    basic_fsm._definition.transitions.append(TransitionDef(basic_fsm.states[0], "InvalidStateType", (0==0)))

    ctx = ValidationContext(basic_fsm._definition)

    validate_transition_types(ctx)

    assert ctx.errors[0].code == "Transition transition state type error"
    assert ctx.errors[0].message == "Invalid destination state Type in Transition"
    assert len(ctx.errors) == 1


def test_transition_condition_non_ExpressionDef_is_Invalid(basic_fsm):
    valid_trans = basic_fsm.state("X", True)
    basic_fsm._definition.transitions.append(TransitionDef(basic_fsm.states[0], valid_trans._definition, "InvalidCondition"))

    ctx = ValidationContext(basic_fsm._definition)

    validate_transition_types(ctx)

    assert ctx.errors[0].code == "Transition transition Expression type error"
    assert ctx.errors[0].message == "Invalid condition expression type in Transition"
    assert len(ctx.errors) == 1


def test_transition_priority_must_be_non_negative_integer(basic_fsm):
    valid_state = basic_fsm.state("X", True)
    valid_parameter = basic_fsm.parameter("G", float)
    valid_condition = valid_parameter == 0
    basic_fsm._definition.transitions.append(TransitionDef(basic_fsm.states[0], valid_state._definition, valid_condition._definition, -15))

    ctx = ValidationContext(basic_fsm._definition)

    validate_transition_types(ctx)

    assert ctx.errors[0].code == "Transition priority type error"
    assert ctx.errors[0].message == "Transition priority must be non negative integer"
    assert len(ctx.errors) == 1

def test_transition_state_must_be_defined_in_same_fsm_instance(basic_fsm):
    another_fsm = FSM("AnotherFSM")
    another_state = another_fsm.state("AnotherState")

    ctx = ValidationContext(basic_fsm._definition)

    ctx.valid_transitions.append(TransitionDef(basic_fsm.states[0], another_state._definition))

    validate_transition_states(ctx)

    assert ctx.errors[0].code == "Transition state reference error"
    assert ctx.errors[0].message == "This state does not exist in this fsm instance."
    assert len(ctx.errors) == 1

# TODO: need to organize 
# def test_transition_parameter_type_non_ParameterExpression_is_Invalid(basic_fsm):

def test_transition_priority_must_be_totally_ordered(basic_fsm):
    valid_state_X = basic_fsm.state("X", False)
    valid_state_B = basic_fsm.state("B", True)
    valid_state_C = basic_fsm.state("C", False)
    valid_param = basic_fsm.parameter("Valid", float)

    valid_state_B.to(valid_state_X).when(valid_param >= 0).priority(20)
    valid_state_B.to(valid_state_C).when(valid_param >= 0).priority(20)

    ctx =ValidationContext(basic_fsm._definition)

    validate_transition_types(ctx)
    validate_transition_priority(ctx)

    assert ctx.errors[0].code == "Transition priority error"
    assert ctx.errors[0].message == "Transition priority is duplicated."
    assert len(ctx.errors) == 1