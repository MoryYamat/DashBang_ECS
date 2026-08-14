from ddknd_fsm.compiler import *
from ddknd_fsm.dsl import *
from ddknd_fsm.authoring import *
import pytest

@pytest.fixture
def basic_fsm():
        
    fsm = FSM("Nominal")

    nom_a = fsm.state("A", True)
    nom_b = fsm.state("B")
    nom_c = fsm.state("C")

    nom_paramA = fsm.parameter("nomParamA", float)
    nom_paramB = fsm.parameter("nomParamB", int)

    nom_condA = nom_paramA <= 10.0
    nom_condB = 20 == nom_paramB
    nom_compose = (nom_condA) & (nom_condB)

    nom_a.to(nom_b).when(nom_condA).priority(120).effect("normto")
    nom_b.to(nom_a).when(nom_condB).priority(200).effect("tonorm")
    nom_b.to(nom_c).when(nom_compose).priority(10).effect("compose")
    # result = compile_fsm(movement)
    # print(nom_a._definition.owner.name)
    # print(nom_b._definition.owner.name)

    return fsm

def test_compile_states_return_ir_states(basic_fsm):
    initial_state_index, compiled_states = compile_states(basic_fsm.states)

    assert initial_state_index == 0
    assert len(compiled_states) == 3
    assert compiled_states[0].name == "A"
    assert compiled_states[1].name == "B"
    assert compiled_states[2].name == "C"
    assert compiled_states[0].index == 0
    assert compiled_states[1].index == 1
    assert compiled_states[2].index == 2

def test_compile_parameters_return_ir_parameters(basic_fsm):
    compiled_parameters = compile_parameters(basic_fsm.parameters)

    assert len(compiled_parameters) == 2
    assert compiled_parameters[0].name == "nomParamA"
    assert compiled_parameters[1].name == "nomParamB"
    assert compiled_parameters[0].index == 0
    assert compiled_parameters[1].index == 1
    assert compiled_parameters[0].value_type == ValueType.FLOAT
    assert compiled_parameters[1].value_type == ValueType.INT


def test_compile_transition_effects_return_ir_effects(basic_fsm):
    compiled_effects = compile_transition_effects(basic_fsm.transitions)

    assert len(compiled_effects) == 3
    assert compiled_effects[0].name == "normto"
    assert compiled_effects[1].name == "tonorm"
    assert compiled_effects[2].name == "compose"
    assert compiled_effects[0].index == 0
    assert compiled_effects[1].index == 1
    assert compiled_effects[2].index == 2

def test_compile_transition_condition_return_condition_expressions(basic_fsm):
    compiled_parameters = compile_parameters(basic_fsm.parameters)
    compiled_conditions = compile_conditions(basic_fsm.transitions, compiled_parameters)

    # nom_paramA <= 10.0
    assert compiled_conditions[0].operator == BinaryOp.LE
    assert compiled_conditions[0].left.parameter_index == compiled_parameters[0].index
    assert compiled_conditions[0].right == IRLiteral(value=10.0, value_type=ValueType.FLOAT)

    # 20 == nom_paramB
    assert compiled_conditions[1].operator == BinaryOp.EQ
    assert compiled_conditions[1].left.parameter_index == compiled_parameters[1].index
    assert compiled_conditions[1].right == IRLiteral(value=20, value_type=ValueType.INT)

    # (nom_condA) & (nom_condB)
    assert compiled_conditions[2].operator == BinaryOp.AND
    assert compiled_conditions[2].left == compiled_conditions[0]
    assert compiled_conditions[2].right == compiled_conditions[1]


def test_compile_transition_return_ir_transitions(basic_fsm):
    _, compiled_states = compile_states(basic_fsm.states)
    compiled_effects = compile_transition_effects(basic_fsm.transitions)
    compiled_parameters = compile_parameters(basic_fsm.parameters)
    compiled_conditions = compile_conditions(basic_fsm.transitions, compiled_parameters)
    compiled_transitions = compile_transitions(basic_fsm.transitions, compiled_states, compiled_parameters, compiled_conditions, compiled_effects)

    assert len(compiled_transitions) == 3
    assert compiled_transitions[0].source_index == 0
    assert compiled_transitions[0].destination_index == 1
    assert compiled_transitions[0].condition_index == 0
    assert compiled_transitions[0].priority == 120
    assert compiled_transitions[0].effect_index == 0

    assert compiled_transitions[1].source_index == 1
    assert compiled_transitions[1].destination_index == 0
    assert compiled_transitions[1].condition_index == 1
    assert compiled_transitions[1].priority == 200
    assert compiled_transitions[1].effect_index == 1

    assert compiled_transitions[2].source_index == 1
    assert compiled_transitions[2].destination_index == 2
    assert compiled_transitions[2].condition_index == 2
    assert compiled_transitions[2].priority == 10
    assert compiled_transitions[2].effect_index == 2

def test_compile_fsm_return_ir_fsm(basic_fsm):
    compiled_result = compile_fsm(basic_fsm._definition)

    assert compiled_result.name == "Nominal"
    assert compiled_result.initial_state_index == 0
    assert len(compiled_result.states) == 3
    assert len(compiled_result.parameters) == 2
    assert len(compiled_result.conditions) == 3
    assert len(compiled_result.effects) == 3
    assert len(compiled_result.transitions) == 3