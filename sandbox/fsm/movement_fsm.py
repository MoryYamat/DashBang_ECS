from  ddknd_fsm.dsl import FSM
from ddknd_fsm.authoring import *

movement = FSM("MovementFSM")

idle = movement.state("Idle", True)
run = movement.state("Run")

# idle.dump()
# run.dump()

# print(idle)
# print(run)

# parameter
speed = movement.parameter("Speed", float)
count = movement.parameter("Count", int)

# speed.dump()

# print(speed)
# print(count)

# Expression (Condition)

eq = speed == 100
ge = 0.01 <= speed
ne = speed != count

compose = (eq) & (ge)

# print(eq)
# print(ge)
# print(ne)
# print(compose)

# print(ne._definition.left.__class__)
# print(ne._definition.right.__class__)

# transition
toRun = idle.to(run).when(eq).priority(20).effect("StartRunning")
toIdle = run.to(idle).when(eq)
# print(toRun)

toRun.dump()