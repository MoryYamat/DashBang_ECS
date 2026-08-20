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

- You can declare and define an FSM using the following syntax.
- You can also generate C++ file 

```py
from  ddknd_fsm.dsl import FSM
from ddknd_fsm.compiler import compile_fsm
from ddknd_fsm.cpp_generator import CppGenerator
from pathlib import Path


# FSM Declaration
movement = dsl.FSM("MovementFSM")

# State Declaration (specify the second argument `True`, if that state is initial state.)
idle = movement.state("Idle", True)
run = movement.state("Run")

# Specify the names and types of external variables for the transition condition expression.
movement_intent = movement.parameter("MovementIntent", float)
grounded = movement.parameter("Grounded", bool)

# In the transition function, you specify the From/To states,
# the condition expression, the priority, and the Effect name.
idle.to(run).when((movement_intent > 0.001) & (grounded == True)).priority(100).effect("move")
run.to(idle).when(movement_intent == 0.0).priority(100).effect("stop")

# Compile the FSM 
compiled_result = compile_fsm(movement._definition)

# Generate the code as a C++ header file
generator = CppGenerator()

# Specify directory and file name to be generated
base_path = Path("sandbox/fsm")
movement_generated_path = generator.generate(result_movement, base_path / "generated", "movement_generated")

# You can check path where the file was generated
print(movement_generated_path)
```

The generated header is expected to look like:

```cpp
#pragma once

#include <ddknd/fsm/runtime/definitions.h>
#include <array>
#include <cstdint>
#include <cassert>

namespace fsm::MovementFSM
{
    enum class MovementFSMState : std::uint8_t
    {
        Idle,
        Run,
    };

    enum class MovementFSMParameterIndex : std::uint16_t
    {
        MovementIntent = 0,
    };

    struct MovementFSMParameters
    {
        float MovementIntent{};
    };

    struct MovementFSMInstance
    {
        MovementFSMState current = MovementFSMState::Idle;
        MovementFSMState previous = MovementFSMState::Idle;
        std::uint32_t revision = 0;
    };


    static bool MovementFSMCondition0(const MovementFSMParameters& parameters)
    {
         return parameters.MovementIntent > 0.001;
    }
    static bool MovementFSMCondition1(const MovementFSMParameters& parameters)
    {
         return parameters.MovementIntent == 0.0;
    }

    using MovementFSMConditionDefinition = ddknd::fsm::ConditionDefinition<MovementFSMParameters>;
    inline constexpr std::array<MovementFSMConditionDefinition, 2>
    MovementFSMConditions = 
    {
        MovementFSMConditionDefinition{&MovementFSMCondition0},
        MovementFSMConditionDefinition{&MovementFSMCondition1},
    };

    inline constexpr std::array<ddknd::fsm::TransitionDefinition, 2>
    MovementFSMTransitions = 
    {
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(MovementFSMState::Idle),
            .destination = static_cast<std::uint32_t>(MovementFSMState::Run),
            .condition = static_cast<std::uint32_t>(0),
            .priority = static_cast<std::uint16_t>(100),
            .effect = static_cast<std::uint16_t>(0),
        },
        ddknd::fsm::TransitionDefinition{
            .source = static_cast<std::uint32_t>(MovementFSMState::Run),
            .destination = static_cast<std::uint32_t>(MovementFSMState::Idle),
            .condition = static_cast<std::uint32_t>(1),
            .priority = static_cast<std::uint16_t>(100),
            .effect = static_cast<std::uint16_t>(1),
        },
    };

    using MovementFSMDefinition = ddknd::fsm::FSMDefinition<MovementFSMParameters>;
    inline constexpr MovementFSMDefinition MovementFSMDef
    {
        .initialState = static_cast<std::uint32_t>(MovementFSMState::Idle),
        .conditions = MovementFSMConditions,
        .transitions = MovementFSMTransitions
    };
}

namespace ddknd::fsm
{
    template<>
    struct ddknd::fsm::FSMTraits<::fsm::MovementFSM::MovementFSMParameters>
    {
        using State = ::fsm::MovementFSM::MovementFSMState;
        using Instance = ::fsm::MovementFSM::MovementFSMInstance;

        static constexpr auto& Definition()
        {
              return ::fsm::MovementFSM::MovementFSMDef;
        };
    };
}
```