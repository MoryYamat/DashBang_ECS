import pytest

from ddknd_fsm.dsl import Expression, FSM, Parameter, State, Transition

def test_fsm_creates_parameter():
    fsm = FSM("TESTFSM")

    speed = fsm.parameter("Speed", float)

    assert(isinstance(speed, Parameter))
    assert speed.name == "Speed"
    assert speed.type == float
    assert fsm.parameters == [speed]


@pytest.mark.parametrize(
    ("method_name", "operator"),
    [
        ("__eq__", "=="),
        ("__ne__", "!="),
        ("__gt__", ">"),
        ("__ge__", ">="),
        ("__lt__", "<"),
        ("__le__", "<="),
    ],
)

def test_parameter_comparison_creates_expression(method_name, operator):
    speed = Parameter("Speed",float)

    comparison_method = getattr(speed, method_name)
    expression = comparison_method(10)

    assert isinstance(expression, Expression)
    assert expression.operator == operator
    assert expression.left is speed
    assert expression.right == 10


def test_and_expression():
    speed = Parameter("Speed", float)

    left = speed > 0.0
    right = speed <= 100

    expression = left & right

    assert expression.operator == "and"
    assert expression.left == left
    assert expression.right == right

def test_or_expression():
    speed = Parameter("Speed", float)

    left = speed > 0.0
    right = speed <= 100

    expression = left | right

    assert expression.operator == "or"
    assert expression.left == left
    assert expression.right == right

def test_nested_boolean_expression():
    speed = Parameter("Speed", float)
    grounded = Parameter("Grounded", bool)
    count = Parameter("Count", int)

    speed_expression = (speed > 0.0)
    grouded_expression = (grounded == True)
    count_expression = (count == 0)

    expression = ( speed_expression & grouded_expression | count_expression )

    left_expression = expression.left
    right_expression = expression.right

    assert expression.operator == "or"

    assert left_expression.operator == "and"
    assert left_expression.left.operator == ">"
    assert left_expression.right.operator == "=="

    assert left_expression.left.left is speed
    assert left_expression.left.right == 0.0

    assert left_expression.right.left is grounded
    assert left_expression.right.right is True

    assert right_expression.operator == "=="
    assert right_expression.left is count
    assert right_expression.right == 0


def test_python_and_is_rejected():
    speed = Parameter("Speed", float)

    with pytest.raises(TypeError):
        (speed > 0.0) and (speed <= 100)

def test_python_or_is_rejected():
    speed = Parameter("Speed", float)

    with pytest.raises(TypeError):
        (speed > 0.0) or (speed <= 100)


def test_fsm_creates_state():
    fsm = FSM("Test")

    testState = fsm.state("TestState", True)

    assert isinstance(testState, State)
    assert testState.fsm is fsm
    assert testState.name == "TestState"
    assert testState.initial is True
    assert fsm.states == [testState]

def test_state_to_creates_transition():
    fsm = FSM("Test")

    firstState = fsm.state("FirstState", True)
    secondState = fsm.state("SecondState")

    transition = firstState.to(secondState)

    assert isinstance(transition, Transition)
    assert transition.source is firstState
    assert transition.destination is secondState
    assert transition in fsm.transitions

def test_when_sets_transition_condition():
    fsm = FSM("TestFSM")
    firstState = fsm.state("FirstState", True)
    secondState = fsm.state("SecondState")

    speed = fsm.parameter("Speed", float)
    condition = speed > 0.01

    transition = firstState.to(secondState)
    result = transition.when(condition)

    assert result is transition
    assert transition.condition is condition

@pytest.mark.parametrize(
    "invalid_condition",
    [
        True,
        False,
        1,
        0.01,
        "speed > 0",
        None,
    ],
)

def test_when_reject_non_expression(invalid_condition):
    fsm = FSM("TestFSM")
    firstState = fsm.state("FirstState", True)
    secondState = fsm.state("SecondState")
    transition = firstState.to(secondState)

    with pytest.raises(TypeError):
        transition.when(invalid_condition)


def test_effect_sets_effect_name():
    fsm = FSM("TestFSM")
    firstState = fsm.state("FirstState", True)
    secondState = fsm.state("SecondState")
    transition = firstState.to(secondState)

    result = transition.effect("TestEffect")

    assert result is transition
    assert transition.effect_name == "TestEffect"

def test_transition_method_chain():
    movement = FSM("MovementFSM")
    idle = movement.state("Idle", initial=True)
    run = movement.state("Run")
    speed = movement.parameter("Speed", float)

    transition = (
        idle
        .to(run)
        .when((speed > 0.01) & (speed <= 100))
        .effect("StartedMoving")
    )

    assert transition.source is idle
    assert transition.destination is run
    assert transition.condition.operator == "and"
    assert transition.effect_name == "StartedMoving"
    assert movement.transitions == [transition]


@pytest.mark.parametrize(
    "invalid_priority_type",
    [
        1.5,
        True,
        "high",
        None,
        [],
    ],
)

def test_priority_rejects_non_integer(invalid_priority_type):
    fsm = FSM("TestFSM")
    first = fsm.state("First")
    second = fsm.state("Second")
    transition = first.to(second)

    with pytest.raises(TypeError):
        transition.priority(invalid_priority_type)

@pytest.mark.parametrize(
    "invalid_priority_value",
    [
        -1,
        -10,
        -100
    ],
)

def test_priority_rejects_negative_integer(invalid_priority_value):
    fsm = FSM("TestFSM")
    first = fsm.state("First")
    second = fsm.state("Second")
    transition = first.to(second)

    with pytest.raises(ValueError):
        transition.priority(invalid_priority_value)