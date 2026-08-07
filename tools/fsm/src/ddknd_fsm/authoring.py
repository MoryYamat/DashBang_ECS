from dataclasses import dataclass, field
from typing import Any

@dataclass
class FSMDef:
    name: str
    states: list["StateDef"] = field(default_factory = list)
    parameters: list["ParameterDef"] = field(default_factory = list)
    transitions: list["TransitionDef"] = field(default_factory = list)

@dataclass
class StateDef:
    name: str
    owner: FSMDef = field(repr=False, compare=False)
    initial: bool = False

@dataclass
class ParameterDef:
    name: str
    owner: FSMDef = field(repr=False, compare=False)
    type_: type

@dataclass
class TransitionDef:
    source: StateDef
    destination: StateDef
    condition: "ExpressionDef | None" = None
    priority: int = 100
    effect_name: str | None = None

# parent class
class ExpressionDef:
    pass

@dataclass
class ParameterExpression(ExpressionDef):
    parameter: ParameterDef

@dataclass
class LiteralExpression(ExpressionDef):
    value: Any
    type_: type

@dataclass(frozen=True)
class BinaryExpression(ExpressionDef):
    operator: str
    left: ExpressionDef
    right: ExpressionDef