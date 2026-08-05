# FSM for DDKND

## Installation

```pwsh
python -m pip install -e .\tools\fsm
```


## Type convertion

| DSL Type | C++ Generated Type |
| --- | --- |
| bool | bool |
| int | std::int32_t |
| UVec2 | ddknd::math::uVec2 |
| UVec3 | ddknd::math::uVec3 |
| FVec2 | ddknd::math::Vec2f |
| FVec3 | ddknd::math::Vec3f |


## Example

You can declare and define an FSM using the following syntax.

```py
from  ddknd_fsm import dsl

# FSM Declaration
movement = dsl.FSM("MovementFSM")

# State Declaration (specify the second argument `True`, if that state is initial state.)
idle = movement.state("Idle", True)
run = movement.state("Run")

# Specify the names and types of external variables for the transition condition expression.
speed = movement.parameter("Speed", float)
grounded = movement.parameter("Grounded", bool)
count = movement.parameter("Count", int)

# In the transition function, you specify the From/To states,
# the condition expression, the priority, and the Effect name.
transition = idle.to(run).when(((speed > 0.01) & (grounded == False) | (Count == 0))).priority(100).effect("StartedMoving")

```