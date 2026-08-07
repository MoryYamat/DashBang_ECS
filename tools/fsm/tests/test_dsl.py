import pytest

from ddknd_fsm.dsl import Expression, FSM, Parameter, State, Transition

from ddknd_fsm.authoring import FSMDef, StateDef, ParameterDef, TransitionDef, ExpressionDef, ParameterExpression, LiteralExpression, BinaryExpression

def test_fsm_creates_parameter():
    fsm = FSM("TestFSM")

    speed = fsm.parameter("Speed", float)

    expectedSpeedDef = ParameterDef("Speed", fsm, float)

    assert(isinstance(speed, Parameter))
    assert speed.name == "Speed"
    assert speed.type == float
    assert fsm.parameters == [expectedSpeedDef]


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
    fsm = FSM("Test")
    speed = fsm.parameter("Speed",float)

    comparison_method = getattr(speed, method_name)
    expression = comparison_method(10)

    assert isinstance(expression, Expression)
    assert expression._definition.operator == operator
    assert expression._definition.left.parameter is fsm._definition.parameters[0]
    assert expression._definition.right == LiteralExpression(10, int)


def test_and_expression():
    fsm = FSM("Test")
    speed = fsm.parameter("Speed", float)

    left = speed > 0.0
    right = speed <= 100

    expression = left & right

    expected_expression_left = BinaryExpression(operator='>', left=ParameterExpression(parameter=ParameterDef("Speed", fsm, float)), right=LiteralExpression(0.0, float))
    expected_expression_right = BinaryExpression(operator='<=', left=ParameterExpression(parameter=ParameterDef("Speed", fsm, float)), right=LiteralExpression(100, int))

    assert expression.operator == "and"
    assert expression.left == expected_expression_left
    assert expression.right == expected_expression_right

def test_or_expression():
    fsm = FSM("Test")
    speed = fsm.parameter("Speed", float)

    left = speed > 0.0
    right = speed <= 100

    expression = left | right

    assert expression.operator == "or"
    assert expression.left == left._definition
    assert expression.right == right._definition

def test_nested_boolean_expression():
    fsm = FSM("Test")
    speed = fsm.parameter("Speed", float)
    grounded = fsm.parameter("Grounded", bool)
    count = fsm.parameter("Count", int)

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

    assert left_expression.left.left.parameter is fsm.parameters[0]
    assert left_expression.left.right == LiteralExpression(0.0, float)

    assert left_expression.right.left.parameter is fsm.parameters[1]
    assert left_expression.right.right == LiteralExpression(True, bool)

    assert right_expression.operator == "=="
    assert right_expression.left.parameter is fsm.parameters[2]
    assert right_expression.right == LiteralExpression(0, int)


def test_python_and_is_rejected():
    fsm = FSM("Test")
    speed = fsm.parameter("Speed", float)

    with pytest.raises(TypeError):
        (speed > 0.0) and (speed <= 100)

def test_python_or_is_rejected():
    fsm = FSM("Test")
    speed = fsm.parameter("Speed", float)

    with pytest.raises(TypeError):
        (speed > 0.0) or (speed <= 100)


def test_fsm_creates_state():
    fsm = FSM("Test")

    testState = fsm.state("TestState", True)
    testStateDef = StateDef("TestState", fsm, True)
    assert isinstance(testState, State)
    assert testState.fsm is fsm._definition
    assert testState.name == "TestState"
    assert testState._definition.initial is True
    assert fsm.states == [testStateDef]

def test_state_to_creates_transition():
    fsm = FSM("Test")

    firstState = fsm.state("FirstState", True)
    secondState = fsm.state("SecondState")

    transition = firstState.to(secondState)

    assert isinstance(transition, Transition)
    assert transition.source is firstState._definition
    assert transition.destination is secondState._definition
    assert transition._definition in fsm.transitions

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

    assert transition.source is idle._definition
    assert transition.destination is run._definition
    assert transition.condition.operator == "and"
    assert transition.effect_name == "StartedMoving"
    assert movement.transitions == [transition._definition]


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