from  ddknd_fsm.dsl import FSM
from ddknd_fsm.authoring import *

movement = FSM("MovementFSM")

idle = movement.state("Idle", True)
run = movement.state("Run")

# print(idle)
# print(run)

# parameter
speed = movement.parameter("Speed", float)
count = movement.parameter("Count", int)

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
# print(toRun)



fsm = FSM("Test")
speed = fsm.parameter("Speed", float)

left = speed > 0.0
right = speed <= 100

expression = left & right

expected_expression_left = BinaryExpression(operator='>', left=ParameterExpression(parameter=ParameterDef("Speed", fsm, float)), right=LiteralExpression(0.0, float))
expected_expression_right = BinaryExpression(operator='<=', left=ParameterExpression(parameter=ParameterDef("Speed", fsm, float)), right=LiteralExpression(100, int))

print(expected_expression_left)
print(expected_expression_right)


print(expression.left)
print(expression.right)



# assert expression.operator == "and"
# assert expression.left == expected_expression_left
# assert expression.right == expected_expression_right