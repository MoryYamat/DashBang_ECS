from  ddknd_fsm import dsl


movement = dsl.FSM("MovementFSM")

idle = movement.state("Idle", True)
run = movement.state("Run")

# To represent a floating-point parameter in the DSL, use a Python float object as a type-specifying symbol.
speed = movement.parameter("Speed", float)
grounded = movement.parameter("Grounded", bool)
count = movement.parameter("Count", int)

toRun = idle.to(run).when((speed > 0.01) & (speed <= 100)).priority(20).effect("StartedMoving")
toIdle = run.to(idle).when(speed <= 0.01).effect("StoppedMoving").priority(100)

expr = (speed > 0.01) & (speed <= 100)
expr2 = (speed > 0.01) & (speed <= 100) | (speed == 50)

print(expr)
print(expr2)
print(toRun)

print(toRun.effect_name)
print(toIdle.effect_name)

print(toIdle.priority)