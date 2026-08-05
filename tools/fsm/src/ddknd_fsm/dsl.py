class FSM:
    def __init__(self, name):
        self.name = name
        self.states = []
        self.parameters = []
        self.transitions = []

    def state(self, name, initial = False):
        _state = State(self, name, initial)
        self.states.append(_state)
        return _state

    def parameter(self, name, type_):
        _parameter = Parameter(name, type_)
        self.parameters.append(_parameter)
        return _parameter

class State:
    def __init__(self, fsm, name, initial = False):
        self.fsm = fsm
        self.name = name
        self.initial = initial

    def to(self, other_state):
        _transition = Transition(self, other_state)
        self.fsm.transitions.append(_transition)
        return _transition

    def __repr__(self):
        return f"State({self.name!r})"


class Parameter:
    def __init__(self, name, type_):
        self.owner = self.fsm
        self.name = name
        self.type = type_

    def __eq__(self, other):
        return Expression("==", self, other)

    def __ne__(self, other):
            return Expression("!=", self, other)
    
    def __gt__(self, other):
        return Expression(">", self, other)

    def __ge__(self, other):
            return Expression(">=", self, other)
    
    def __lt__(self, other):
        return Expression("<", self, other)

    def __le__(self, other):
            return Expression("<=", self, other)

    def __repr__(self):
        return f"Parameter({self.name}, {self.type.__name__})"


class Expression:
    def __init__(self, operator, left = None, right = None):
        self.operator = operator
        self.left = left
        self.right = right

    def __and__(self, other):
        return Expression("and", self, other)

    def __or__(self, other):
        return Expression("or", self, other)

    def __bool__(self):
        raise TypeError("DSL式では 'and'/'or' ではなく '&'/'|' を使用してください")

    def __repr__(self):
        return (f"Expression({self.operator!r}, {self.left!r}, {self.right!r})")


class Transition:
    def __init__(self, source, destination):
        self.source = source
        self.destination = destination
        self.condition = None
        self.effect_name = None
        self.priority_ = 100

    def when(self, expression):
        if not isinstance(expression, Expression):
            raise TypeError("when()にはDSL式を指定してください")

        self.condition = expression
        return self

    def priority(self,value):
        if isinstance(value, bool) or not isinstance(value, int):
            raise TypeError("priorityには整数を指定してください")

        if value < 0:
            raise ValueError("priorityには0以上の整数を指定してください")
        
        self.priority_ = value
        return self

    def effect(self, name):
        self.effect_name = name
        return self

    def __repr__(self):
        return ("Transition("f"{self.source.name!r} -> {self.destination.name!r}, "
                f"condition={self.condition}, "
                f"priority={self.priority_}, "
                f"effect={self.effect_name})")



class UVec2:
    pass

class UVec3:
    pass

class FVec2:
    pass

class FVec3:
    pass
