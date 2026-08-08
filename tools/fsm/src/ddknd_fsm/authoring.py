from dataclasses import dataclass, field
from typing import Any

@dataclass
class FSMDef:
    name: str
    states: list["StateDef"] = field(default_factory = list)
    parameters: list["ParameterDef"] = field(default_factory = list)
    transitions: list["TransitionDef"] = field(default_factory = list)

    def dump_states(self):
        v = ""
        for s in self.states:
            v += s.name + " "
        print("StateDef list: [ " + v + "]")

    def dump_parameters(self):
        v = ""
        for p in self.parameters:
            v += p.name + " "
        print("ParameterDef list: [ " + v + "]")

    def dump_transitions(self):
        print ("Transition list: ")
        for t in self.transitions:
            t.dump(1)

@dataclass
class StateDef:
    name: str
    owner: FSMDef = field(repr=False, compare=False)
    initial: bool = False

    def dump(self, attr: str = "", indent: int = 0):
        prefix = " " * indent
        print(f"{prefix}StateDef" + attr +  f": {self.name} ")

@dataclass
class ParameterDef:
    name: str
    owner: FSMDef = field(repr=False, compare=False)
    type_: type

    def dump(self, indent: int = 0):
        prefix = " " * indent
        print(prefix + "ParameterDef" + f": {self.type_.__name__} {self.name}")

@dataclass
class TransitionDef:
    source: StateDef
    destination: StateDef
    condition: "ExpressionDef | None" = None
    priority: int = 100
    effect_name: str | None = None

    def dump(self, indent: int = 0):
        prefix = " " * indent
        prefix_more = " " * (indent + 1)
        print(prefix + "TransitionDef: ")
        self.source.dump("(src)", indent+1)
        self.destination.dump("(dst)", indent+1)
        self.condition.dump(indent+1)
        print(prefix_more + "effect_name: " + self.effect_name)

# parent class
class ExpressionDef:
    pass

@dataclass
class ParameterExpression(ExpressionDef):
    parameter: ParameterDef

    def dump(self, operand : str = "", indent: int = 0):
        prefix = " " * indent
        print(prefix + operand)
        self.parameter.dump(indent + 1)

@dataclass
class LiteralExpression(ExpressionDef):
    value: Any
    type_: type

    def dump(self, operand : str = "", indent: int = 0):
        prefix = " " * indent
        prefix_more = " " * (indent + 1)
        print(prefix + operand + "\n" + prefix_more + "Value: " + f"{self.value}")

@dataclass(frozen=True)
class BinaryExpression(ExpressionDef):
    operator: str
    left: ExpressionDef
    right: ExpressionDef

    def dump(self, indent: int = 0):
        prefix = " " * indent
        prefix_double = " " * (indent + 1)
        print(prefix + "BinaryExpression: ")
        print(prefix_double + "Operator: " + self.operator)
        self.left.dump("left: ", indent + 1)
        self.right.dump("right: ",indent + 1)