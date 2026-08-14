from ddknd_fsm.authoring import *
from ddknd_fsm.validation import validate_fsm
from ddknd_fsm.ir import *


def compile_fsm(definition: FSMDef):
    if not isinstance(definition, FSMDef):
        raise TypeError("argument must be FSMDef.")

    validation_result = validate_fsm(definition)


    if not validation_result.ok:
        for e in validation_result.errors:
            print(e)

    initial_state, compiled_states = compile_states(definition.states)
    compiled_parameters = compile_parameters(definition.parameters)
    compiled_effects = compile_transition_effects(definition.transitions)
    compiled_conditions = compile_conditions(definition.transitions, compiled_parameters)
    compiled_transitions = compile_transitions(definition.transitions, compiled_states, compiled_parameters, compiled_conditions, compiled_effects)

    print("compiled_states:")
    for s in compiled_states:
        print(s)

    print("compiled_parameters:")
    for p in compiled_parameters:
        print(p)

    print("compiled_effects:")
    for p in compiled_effects:
        print(p)

    print("compiled_conditions")
    for p in compiled_conditions:
        print(p)

    print("compiled_transitions:")
    for p in compiled_transitions:
        print(p)


    compile_result = FSMIR(name=definition.name,
                      initial_state_index=initial_state,
                      states=compiled_states,
                      parameters=compiled_parameters,
                      transitions=compiled_transitions,
                      conditions=compiled_conditions,
                      effects=compiled_effects)

    return compile_result


def compile_states(states : list[StateDef]):

    initial_state_index = -1
    states_result = []

    for i, s in enumerate(states):
        if s.initial == True:
            initial_state_index = i
        states_result.append(IRState(index=i, name = s.name))

    if initial_state_index == -1:
        raise ValueError("Failed to resolve initial state index.")

    return initial_state_index, states_result

# int, bool, float, UVec2, UVec3, FVec2, FVec3
def handle_type(type_: type):
    if type_ == int:
        return ValueType.INT
    elif type_ == bool:
        return ValueType.BOOL
    elif type_ == float:
        return ValueType.FLOAT
    elif type_ == UVec2:
        return ValueType.UVEC2
    elif type_ == UVec3:
        return ValueType.UVEC3
    elif type_ == FVec2:
        return ValueType.FVEC2
    elif type_ == FVec3:
        return ValueType.FVEC3
    else:
        raise TypeError("Invalid type.")

def handle_operator(operator: str):
    if operator == "==":
        return BinaryOp.EQ
    elif operator == "!=":
        return BinaryOp.NE
    elif operator == ">":
        return BinaryOp.GT
    elif operator == ">=":
        return BinaryOp.GE
    elif operator == "<":
        return BinaryOp.LT
    elif operator == "<=":
        return BinaryOp.LE
    elif operator == "and":
        return BinaryOp.AND
    elif operator == "or":
        return BinaryOp.OR
    else:
        raise TypeError("Invalid type.")


def compile_parameters(parameters: list[ParameterDef]):
    result = []

    for i, p in enumerate(parameters):
        result.append(IRParameter(index=i, name=p.name, value_type=handle_type(p.type_)))

    return result

def compile_transition_effects(transitions: list[TransitionDef]):
    result = []

    for i, t in enumerate(transitions):
        result.append(IREffect(index=i, name=t.effect_name))

    return result

def resolve_parameter_ref(parameterExpression : ParameterExpression, compiled_parameters : list[IRParameter]) -> IRParameterRef:
    for p in compiled_parameters:
        if p.name == parameterExpression.parameter.name:
            return p.index

def compile_expression(expression: ExpressionDef, compiled_parameters : list[IRParameter]) -> IRExpression:
    if isinstance(expression, BinaryExpression):
        operator = handle_operator(expression.operator)
        return (IRBinaryExpression(operator,
            compile_expression(expression.left, compiled_parameters),
            compile_expression(expression.right, compiled_parameters))
        )
    elif isinstance(expression, LiteralExpression):
        return IRLiteral(expression.value, handle_type(expression.type_))
    elif isinstance(expression, ParameterExpression):
        return IRParameterRef(resolve_parameter_ref(expression, compiled_parameters))

def compile_conditions(transitions: list[TransitionDef], parameters : list[IRParameter]) -> list[IRExpression]:
    result = []
    for t in transitions:
        result.append(compile_expression(t.condition, parameters))

    return result

def compile_transitions(transitions: list[TransitionDef], states : list[IRState], parameters : list[IRParameter], conditions : list[IRExpression], effects : list[IREffect]):
    result = []
    for t in transitions:
        condition_expression = compile_expression(t.condition, parameters)

        source_index = -1
        destination_index = -1
        for state in states:
            if state.name == t.source.name:
                source_index = state.index
            if state.name == t.destination.name:
                destination_index = state.index

        if source_index == -1 or destination_index == -1:
            raise ValueError("Failed to resolve the state name.")

        condition_index = next((j for j, c in enumerate(conditions) if c == condition_expression), -1)
        if condition_index == -1:
            raise ValueError("Failed to resolve the condition index.")
        
        effect_index = next((k for k, e in enumerate(effects) if e.name == t.effect_name), -1)
        if effect_index == -1:
            raise ValueError("Failed to resolve the effect index.")

        result.append(IRTransition(source_index=source_index, destination_index=destination_index, condition_index=condition_index, priority=t.priority, effect_index=effect_index))

    return result