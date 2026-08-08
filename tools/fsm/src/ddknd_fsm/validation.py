from ddknd_fsm.authoring import *
import textwrap

# policy: 
# - verify constraints that were validated at the DSL frontend layer.
# - continue the verification if an error is detected during the process.

# verification of structural constraints
# - There is at least one state.
# verification of referential validatity

# verification of semantic constraints

# verification of Transition constraints

@dataclass
class ValidationError:
    code: str
    message: str
    details: str

    def __repr__(self):
        prefix = "- '" + self.code + "': "+ self.message + " --> ("
        indent = " " * len(prefix)
        code = self.code.replace("\n", "\n" + indent)
        # return ("- " + self.message + f" --> ({self.code})")
        return prefix + code + ")"

@dataclass
class ValidationResult:
    errors: list[ValidationError]
    fsmName: str = ""

    @property
    def ok(self)->bool:
        return not self.errors

    def diagnostics(self):
        if self.ok :
            print(f"{self.fsmName}" " Validation successful.")
        else:
            print("diagnostics " f"({self.fsmName}): ")
            for e in self.errors:
                print(e)


@dataclass
class ValidationContext:
    fsm: FSMDef
    errors: list[ValidationError] = field(default_factory=list)

    valid_states: list[StateDef] = field(default_factory=list)
    valid_parameters: list[ParameterDef] = field(default_factory=list)
    valid_transitions: list[TransitionDef] = field(default_factory=list)


def validate_fsm(fsm: FSMDef)->ValidationResult:
    ctx = ValidationContext(fsm, [])

    # structural
    validate_state_structure(ctx)
    validate_parameters_structure(ctx)
    validate_transition_structure(ctx)

    # referential
    validate_state_referential(ctx)

    # semantic
    validate_states_semantic(ctx)
    validate_parameters_semantic(ctx)
    validate_transitions_semantic(ctx)

    return ValidationResult(ctx.errors, fsm.name)


def validate_state_structure(ctx: ValidationContext):
    validate_state_type(ctx)

def validate_states_semantic(ctx : ValidationContext):
    validate_unique_names(ctx)
    validate_initial_state(ctx)


def validate_state_type(ctx : ValidationContext):
    for state in ctx.fsm.states:
        if not isinstance(state, StateDef):
            ctx.errors.append(ValidationError("Invalid state Type","state type must be StateDef.", repr(state)))
            continue
        ctx.valid_states.append(state)

def validate_state_referential(ctx: ValidationContext):
    for state in ctx.valid_states:
        if state.owner is not ctx.fsm:
            ctx.errors.append(ValidationError("Invalid fsm reference", "StateDef must have reference to the same FSM Definition.", repr(state)))

def validate_unique_names(ctx : ValidationContext):
    visited = []
    for state in ctx.fsm.states:
        if state.name in visited:
            ctx.errors.append(ValidationError("State name duplication error", "There is a duplicate state name.", repr(state.name)))
        visited.append(state.name)


def validate_initial_state(ctx : ValidationContext):
    count = 0
    initial_states = ""
    for state in ctx.fsm.states:
        if state.initial is True:
            count += 1
            initial_states += f"{count}: " + state.name + " "
        if count == 0:
            ctx.errors.append(ValidationError("Initial state count error", "At least one initial state is required.", repr(ctx.fsm)))
        elif count >= 2:
            ctx.errors.append(ValidationError("Initial state count error", "There are two or more initial states.", repr(initial_states)))

def validate_parameters_structure(ctx : ValidationContext):
    validate_parameter_types(ctx)

def validate_parameters_semantic(ctx : ValidationContext):
    validate_parameter_names(ctx)

def validate_parameter_names(ctx : ValidationContext):
    visited = []
    for parameter in ctx.fsm.parameters:
        if parameter.name in visited:
            ctx.errors.append(ValidationError("Parameter name duplication error","There is a duplicate parameter name.", repr(parameter)))
        visited.append(parameter.name)

def validate_parameter_types(ctx : ValidationContext):
    for parameter in ctx.fsm.parameters:
        if not isinstance(parameter, ParameterDef):
            ctx.errors.append(ValidationError("Parameter type error", "Invalid parameter type.", repr(parameter)))
            continue
        if parameter.type_ not in [int, float, bool, UVec2, UVec3, FVec2, FVec3]:
            ctx.errors.append(ValidationError("Parameter type error","Invalid parameter type.", repr(parameter)))
            continue
        ctx.valid_parameters.append(parameter)


def validate_transition_structure(ctx : ValidationContext):
    validate_transition_types(ctx)
    validate_transition_parameters(ctx)

def validate_transitions_semantic(ctx : ValidationContext):
    validate_transition_states(ctx)
    validate_transition_priority(ctx)

def validate_transition_types(ctx : ValidationContext):
    for transition in ctx.fsm.transitions:
        if not isinstance(transition, TransitionDef):
            ctx.errors.append(ValidationError("Transition type error", "Invalid Transition Type", repr(transition)))
            continue
        if not isinstance(transition.source, StateDef):
            ctx.errors.append(ValidationError("Transition transition state type error","Invalid source state Type in Transition", repr(transition.source)))
            continue
        if not isinstance(transition.destination, StateDef):
            ctx.errors.append(ValidationError("Transition transition state type error", "Invalid destination state Type in Transition", repr(transition.destination)))
            continue
        if not isinstance(transition.condition, ExpressionDef):
            ctx.errors.append(ValidationError("Transition transition Expression type error","Invalid condition expression type in Transition", repr(transition.condition)))
            continue
        if not isinstance(transition.priority, int) or transition.priority < 0:
            ctx.errors.append(ValidationError("Transition priority type error", "Transition priority must be non negative integer", (repr(transition.source) + " -> "+ repr(transition.destination) + ": priority=" + repr(transition.priority))))
            continue

        ctx.valid_transitions.append(transition)

def validate_transition_states(ctx : ValidationContext):
    for transition in ctx.valid_transitions:

        if transition.source not in ctx.fsm.states:
            ctx.errors.append(ValidationError("Transition state reference error", "This state is not exists in this fsm instance.", repr(transition.source)))

        if transition.destination not in ctx.valid_states:
            ctx.errors.append(ValidationError("Transition state reference error", "This state does not exist in this fsm instance.", repr(transition.destination)))

def classify_expression_and_validate_operand(exp: ExpressionDef, ctx: ValidationContext) -> list[ExpressionDef]:
    if isinstance(exp, BinaryExpression):
        if not exp.operator in  {"==", "!=", ">" , ">=", "<", "<=", "and", "or"}:
            ctx.errors.append(ValidationError("Expression operator type error","Invalid operator.", repr(exp.operator)))
        return (
            classify_expression_and_validate_operand(exp.left, ctx)
            + classify_expression_and_validate_operand(exp.right, ctx)
        )
    elif isinstance(exp, LiteralExpression):
        return [exp]
    elif isinstance(exp, ParameterExpression):
        return [exp]

def validate_transition_parameters(ctx: ValidationContext):
    for exp in ctx.valid_transitions:
        res = classify_expression_and_validate_operand(exp.condition, ctx)

        for item in res:
            if isinstance(item, ParameterExpression) and item.parameter not in ctx.valid_parameters:
                ctx.errors.append(ValidationError("ExpressionParameter reference error", "This is parameter does not exist in this fsm instance.", repr(item.parameter)))


def validate_transition_priority(ctx: ValidationContext):
    for i, transition in enumerate(ctx.valid_transitions):
        for j in range(i, len(ctx.valid_transitions)):
            second_transition = ctx.valid_transitions[j]
            if (second_transition is not transition 
                and second_transition.source == transition.source
                and second_transition.priority == transition.priority
                ):
                first_transition_info = repr(transition.source) + " -> " + repr(transition.destination) + ": priority=" + repr(transition.priority)
                second_transition_info = repr(second_transition.source) + " -> " + repr(second_transition.destination) + ": priority=" + repr(second_transition.priority)
                ctx.errors.append(ValidationError("Transition priority error", "Transition priority is duplicated."
                                                  , first_transition_info + "\n" + 
                                                     second_transition_info))


