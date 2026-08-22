from  ddknd_fsm.dsl import FSM
from ddknd_fsm.compiler import compile_fsm
from ddknd_fsm.cpp_generator import CppGenerator
from pathlib import Path

movement = FSM("MovementFSM")

idle = movement.state("Idle", True)
run = movement.state("Run")

movement_intent = movement.parameter("MovementIntent", float)

idle.to(run).when(movement_intent > (1e-5)).priority(100).effect("move")
run.to(idle).when(movement_intent == 0.0).priority(100).effect("stop")

result_movement = compile_fsm(movement._definition)

generator = CppGenerator()

base_path = Path("sandbox/game/include/game/player/fsm/")
# base_path = Path("sandbox/fsm/")
movement_generated_path = generator.generate(result_movement, base_path / "generated", "movement")

print(movement_generated_path)