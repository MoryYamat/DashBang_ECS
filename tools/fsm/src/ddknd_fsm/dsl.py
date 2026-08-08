from .authoring import FSMDef, StateDef, ParameterDef, TransitionDef, ExpressionDef, ParameterExpression, LiteralExpression, BinaryExpression

class FSM:
    def __init__(self, name: str):
        self._definition = FSMDef(name=name)

    @property
    def definition(self)->FSMDef:
        return self._definition

    @property
    def name(self)->str:
        return self._definition.name

    def state(self, name: str, initial: bool = False) -> "State":
        definition = StateDef(name=name, owner=self._definition, initial=initial)

        self._definition.states.append(definition)
        return State(definition)

    def parameter(self, name: str, type_: type) -> "Parameter":
        definition = ParameterDef(name=name, owner=self._definition, type_= type_)
        self._definition.parameters.append(definition)
        return Parameter(definition)

    @property
    def parameters(self)->list:
        return self._definition.parameters

    @property
    def states(self)->list:
        return self._definition.states

    @property
    def transitions(self)->list:
        return self._definition.transitions

    def dump_states(self):
        self._definition.dump_states()

    def dump_parameters(self):
        self._definition.dump_parameters()

    def dump_transitions(self):
        self._definition.dump_transitions()

class State:
    def __init__(self, definition: StateDef):
        self._definition = definition

    @property
    def name(self)->str:
        return self._definition.name

    @property
    def fsm(self):
        return self._definition.owner

    def to(self, other_state) -> "Transition":
        definition = TransitionDef(
            source=self._definition,
            destination=other_state._definition
        )
        self._definition.owner.transitions.append(definition)
        return Transition(definition)

    def __repr__(self):
            return ("FSM: \""f"{self._definition.owner.name}\", State(name: \""f"{self._definition.name}\", initial: "f"{self._definition.initial})")

    def dump(self):
        self._definition.dump()

class Parameter:
    def __init__(self, definition: ParameterDef):
        self._definition = definition

    @property
    def definition(self)->ParameterDef:
        return self._definition

    @property
    def name(self) -> str:
        return self._definition.name

    @property
    def type(self) -> type:
        return self._definition.type_

    def _to_Expression(self, value = None):
        if value is None and isinstance(self._definition, ParameterDef):
            return ParameterExpression(self._definition)
        if isinstance(value, (int, bool, float, UVec2, UVec3, FVec2, FVec3)):
            return LiteralExpression(value, type(value))
        elif isinstance(value, Parameter):
            return ParameterExpression(value._definition)

    def __eq__(self, other):
        _binary_expression = BinaryExpression("==", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __ne__(self, other):
        _binary_expression = BinaryExpression("!=", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __gt__(self, other):
        _binary_expression = BinaryExpression(">", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __ge__(self, other):
        _binary_expression = BinaryExpression(">=", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __lt__(self, other):
        _binary_expression = BinaryExpression("<", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __le__(self, other):
        _binary_expression = BinaryExpression("<=", self._to_Expression(), self._to_Expression(other))
        return Expression(_binary_expression)

    def __bool__(self):
        raise TypeError("DSL式では 'and'/'or' ではなく '&'/'|' を使用してください")

    def __repr__(self):
        return ("FSM: \""f"{self._definition.owner.name}\", parameter(name: \""f"{self._definition.name}\", type: "f"{self._definition.type_.__name__})")

    def dump(self):
        self._definition.dump()

class Expression:
    def __init__(self, other : ExpressionDef):
        self._definition = other

    def _to_Expression(self, value):
        if isinstance(value, Parameter):
            return ParameterExpression(value._definition)
        elif isinstance(value, Expression):
            return value._definition
        elif isinstance(value, (int, bool, float, UVec2, UVec3, FVec2, FVec3)):
            return LiteralExpression(value, type(value))
        else:
            raise TypeError("条件式の項の型が不正です。")


    def __and__(self, other):
        _right = self._to_Expression(other)
        composed = BinaryExpression("and", self._definition, _right)
        return Expression(composed)

    def __or__(self, other):
        _right = self._to_Expression(other)
        composed = BinaryExpression("or", self._definition, _right)
        return Expression(composed)

    def __bool__(self):
        raise TypeError("DSL式では 'and'/'or' ではなく '&'/'|' を使用してください")

    def __repr__(self):
        return ("Expression(operator: \""f"{self._definition.operator}\",\n"
                "   left("f"{self._definition.left}),\n"
                "   right("f"{self._definition.right}))")

    @property
    def operator(self):
        return self._definition.operator if isinstance(self._definition, BinaryExpression) else None
    @property
    def left(self):
        return self._definition.left if isinstance(self._definition, BinaryExpression) else None
    @property
    def right(self):
            return self._definition.right if isinstance(self._definition, BinaryExpression) else None


class Transition:
    def __init__(self, definition : TransitionDef):
        self._definition = definition

    def when(self, condition : Expression):
        if not isinstance(condition, Expression):
            raise TypeError("when()にはDSL式を指定してください")

        self._definition.condition = condition._definition
        return self

    def priority(self, value : int):
        if isinstance(value, bool) or not isinstance(value, int):
            raise TypeError("priorityには整数を指定してください")
            
        if value < 0:
            raise ValueError("priorityには0以上の整数を指定してください")

        self._definition.priority = value
        return self

    def effect(self, effect_name: str):
        if isinstance(effect_name, str):
            self._definition.effect_name = effect_name
            return self
        else:
            raise TypeError("'effect'は文字列で指定してください")

    def __repr__(self):
        return ("Transition("f"{self._definition.source}" " -> " f"{self._definition.destination}, "
                "priority="f"{self._definition.priority}, "
                "effect="f"{self._definition.effect_name}, " 
                "condition="f"{self._definition.condition})")

    def dump(self):
        self._definition.dump()

    @property
    def source(self):
        return self._definition.source

    @property
    def destination(self):
        return self._definition.destination

    @property
    def condition(self):
        return self._definition.condition

    @property
    def effect_name(self):
        return self._definition.effect_name

class UVec2:
    pass

class UVec3:
    pass

class FVec2:
    pass

class FVec3:
    pass
