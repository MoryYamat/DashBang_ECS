from  ddknd_fsm.dsl import FSM
from ddknd_fsm.authoring import *

from ddknd_fsm.compiler import *

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

skill = FSM("SkillFSM")

none = skill.state("None")


hell = skill.parameter("Hell", float)

movement.parameters.append(hell)

# transition
toRun = idle.to(run).when(eq).priority(20).effect("StartRunning")
# toIdle = run.to(none).when(eq)
toIdle_nonsense = run.to(skill).when(ge)
toIdle_invalid = run.to(none).when(compose)
# print(toRun)

# toRun.dump()


idle_dup = movement.state("Idle")
speed_dup = movement.parameter("Speed", float)

invalid_cond = hell <= 10

zip = movement.state("zip")

toZip = idle.to(zip).when(invalid_cond).priority(20).effect("hello_zip")

# print(speed_dup._definition.type_)
# print(float)

# result = validate_fsm(movement._definition)

# print(result.ok)
# result.diagnostics()

print("nominal")

nominal = FSM("Nominal")

nom_a = nominal.state("A", True)
nom_b = nominal.state("B")
nom_c = nominal.state("C")

nom_param = nominal.parameter("nomParam", float)

nom_condA = nom_param <= 10.0
nom_condB = 20 == nom_param

nom_a.to(nom_b).when(nom_condA).priority(120).effect("normto")
nom_b.to(nom_a).when(nom_condB).priority(200).effect("tonorm")
nom_b.to(nom_c).when((nom_condB) & (nom_condA)).priority(10).effect("compose")
# result = compile_fsm(movement)
# print(nom_a._definition.owner.name)
# print(nom_b._definition.owner.name)

result_nom = compile_fsm(nominal._definition)