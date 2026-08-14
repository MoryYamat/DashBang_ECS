from dataclasses import dataclass
from enum import Enum, auto
from typing import Any

class ValueType(Enum):
    BOOL = auto()
    INT = auto()
    FLOAT = auto()
    UVEC2 = auto()
    UVEC3 = auto()
    FVEC2 = auto()
    FVEC3 = auto()

class BinaryOp(Enum):
    EQ = auto()
    NE = auto()
    GT = auto()
    GE = auto()
    LT = auto()
    LE = auto()
    AND = auto()
    OR = auto()

@dataclass(frozen=True)
class IRState:
    index: int
    name: str

@dataclass(frozen=True)
class IRParameter:
    index: int
    name: str
    value_type: ValueType

class IRExpression:
    pass

@dataclass(frozen=True)
class IRParameterRef(IRExpression):
    parameter_index: int

@dataclass(frozen=True)
class IRLiteral(IRExpression):
    value: Any
    value_type: ValueType

@dataclass(frozen=True)
class IRBinaryExpression:
    operator: BinaryOp
    left: IRExpression
    right: IRExpression

@dataclass(frozen=True)
class IRTransition:
    source_index: int
    destination_index: int
    condition_index: int
    priority: int
    effect_index: int | None

@dataclass(frozen=True)
class IREffect:
    index: int
    name: str

@dataclass(frozen=True)
class FSMIR:
    name: str
    initial_state_index: int

    states: tuple[IRState, ...]
    parameters: tuple[IRParameter, ...]
    transitions: tuple[IRTransition, ...]
    conditions: tuple[IRExpression, ...]
    effects: tuple[IREffect, ...]

    # def dump():