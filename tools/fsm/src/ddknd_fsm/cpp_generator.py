from ddknd_fsm.ir import *
from pathlib import Path

class CppGenerator:
    def generate(self, ir : FSMIR, output_dir : Path, file_name : str | None = None) -> Path:
        code = self._render(ir)

        output_dir.mkdir(parents=True, exist_ok=True)

        if file_name is None:
            file_name = f"{ir.name}_generated.h"

        output_path = output_dir / (file_name + ".h")
        output_path.write_text(code, encoding="utf-8")

        return output_path

    def _render(self, ir : FSMIR) -> str:
        lines : list[str] = []

        lines.append("#pragma once")
        lines.append("")
        lines.append("#include <ddknd/fsm/runtime/definitions.h>")
        lines.append("#include <array>")
        lines.append("#include <cstdint>")
        lines.append("#include <cassert>")
        lines.append("")

        namespace_body = []

        namespace_body.extend(generate_state_enum(ir))
        namespace_body.append("")
        namespace_body.extend(generate_parameter_index(ir))
        namespace_body.append("")
        namespace_body.extend(generate_parameters(ir))
        namespace_body.append("")
        namespace_body.extend(generate_FSM_Instance(ir))
        namespace_body.append("")
        namespace_body.append("")
        namespace_body.extend(generate_condition_functions(ir))
        namespace_body.append("")
        namespace_body.extend(generate_condition_definition(ir))
        namespace_body.append("")
        namespace_body.extend(generate_transitions(ir))
        namespace_body.append("")
        namespace_body.extend(generate_definition(ir))


        lines.append(f"namespace fsm::{ir.name}")
        lines.append("{")
        lines.extend(indent(namespace_body))
        lines.append("}")
        
        lines.append("")
        lines.extend(generate_traits(ir))


        return "\n".join(lines)

def generate_state_enum(ir: FSMIR) -> list[str]:
    lines = []

    lines.append(
        f"enum class {ir.name}State : std::uint8_t"
    )
    lines.append("{")

    body = []

    for state in ir.states:
        body.append(f"{state.name},")

    lines.extend(indent(body))

    lines.append("};")

    return lines

def generate_parameter_index(ir : FSMIR) -> list[str]:
    lines = []

    lines.append(
        f"enum class {ir.name}ParameterIndex : std::uint16_t"
    )
    lines.append("{")

    body = []

    for i, p in enumerate(ir.parameters):
        body.append(f"{p.name} = {i},")

    lines.extend(indent(body))
    lines.append("};")

    return lines

def generate_parameters(ir: FSMIR) -> list[str]:
    lines = []

    lines.append(
        f"struct {ir.name}Parameters"
    )
    lines.append("{")

    body = []

    for parameter in ir.parameters:
        body.append(f"{value_type_enum_to_str(parameter.value_type)} {parameter.name}{{}};")

    lines.extend(indent(body))

    lines.append("};")

    return lines

def generate_FSM_Instance(ir: FSMIR) -> list[str]:
    lines = []

    lines.append(
        f"struct {ir.name}Instance"
    )
    lines.append("{")

    body = [
        f"{ir.name}State current = {ir.name}State::{ir.states[ir.initial_state_index].name};",
        f"{ir.name}State previous = {ir.name}State::{ir.states[ir.initial_state_index].name};",
        "std::uint32_t revision = 0;",
    ]

    lines.extend(indent(body))

    lines.append("};")

    return lines

def generate_condition_functions(ir :FSMIR) -> list[str]:
    lines = []


    for i, c in enumerate(ir.conditions):
        lines.append(f"static bool {ir.name}Condition{i}(const {ir.name}Parameters& parameters)")
        lines.append("{")
        lines.append(f"     return {generate_condition_expression_from_ir(c, ir)};")
        lines.append("}")

    return lines

def generate_condition_definition(ir) -> list[str]:
    lines = []

    lines.append(f"using {ir.name}ConditionDefinition = ddknd::fsm::ConditionDefinition<{ir.name}Parameters>;")
    lines.append(f"inline constexpr std::array<{ir.name}ConditionDefinition, {len(ir.conditions)}>")
    lines.append(f"{ir.name}Conditions = ")
    lines.append("{")

    body = []

    for i, c in enumerate(ir.conditions):
        body.append(f"{ir.name}ConditionDefinition{{&{ir.name}Condition{i}}},")

    lines.extend(indent(body))
    lines.append("};")

    return lines

def generate_transitions(ir : FSMIR) -> list[str]:
    lines = []

    lines.append(f"inline constexpr std::array<ddknd::fsm::TransitionDefinition, {len(ir.transitions)}>")
    lines.append(f"{ir.name}Transitions = ")
    lines.append("{")

    body = []

    for t in ir.transitions:
        transition = [
            "ddknd::fsm::TransitionDefinition{",
            f".source = static_cast<std::uint32_t>("
            f"{ir.name}State::{ir.states[t.source_index].name}),",
            f".destination = static_cast<std::uint32_t>("
            f"{ir.name}State::{ir.states[t.destination_index].name}),",
            f".condition = static_cast<std::uint32_t>({t.condition_index}),",
            f".priority = static_cast<std::uint16_t>({t.priority}),",
            f".effect = static_cast<std::uint16_t>({t.effect_index}),",
            "},",
        ]

        body.append(transition[0])
        body.extend(indent(transition[1:-1]))
        body.append(transition[-1])

    lines.extend(indent(body))
    lines.append("};")

    return lines

def generate_definition(ir :FSMIR) -> list[str]:
    lines = []

    lines.append(f"using {ir.name}Definition = ddknd::fsm::FSMDefinition<{ir.name}Parameters>;")
    lines.append(f"inline constexpr {ir.name}Definition {ir.name}Def")
    lines.append("{")

    body = [
        f".initialState = static_cast<std::uint32_t>({ir.name}State::{ir.states[ir.initial_state_index].name}),",
        f".conditions = {ir.name}Conditions,",
        f".transitions = {ir.name}Transitions",
    ]

    lines.extend(indent(body))
    lines.append("};")

    return lines

def generate_traits(ir : FSMIR) -> list[str]:
    body = []
    body.append("template<>")
    body.append(f"struct ddknd::fsm::FSMTraits<::fsm::{ir.name}::{ir.name}Parameters>")
    body.append("{")

    struct_body = [
        f"using State = ::fsm::{ir.name}::{ir.name}State;",
        f"using Instance = ::fsm::{ir.name}::{ir.name}Instance;",
        "",
        f"static constexpr auto& Definition()",
        "{",
        f"      return ::fsm::{ir.name}::{ir.name}Def;",
        "};",
    ]

    body.extend(indent(struct_body))
    body.append("};")

    return [
        "namespace ddknd::fsm",
        "{",
        *indent(body),
        "}"
    ]

def value_type_enum_to_str(type: ValueType):
    match type:
        case ValueType.INT:
            return "int"
        case ValueType.BOOL:
            return "bool"
        case ValueType.FLOAT:
            return "float"
        case ValueType.UVEC2:
            return "uVec2"
        case ValueType.UVEC3:
            return "uVec3"
        case ValueType.FVEC2:
            return "Vec2f"
        case ValueType.FVEC3:
            return "Vec3f"
        case _:
            raise ValueType("Invalid enum.")


def generate_condition_expression_from_ir(condition : IRBinaryExpression, ir : FSMIR) ->str:
    if isinstance(condition, IRBinaryExpression):
        return (
            f"{generate_condition_expression_from_ir(condition.left, ir)} {resolve_operator_from_ir(condition.operator)} {generate_condition_expression_from_ir(condition.right, ir)}"
        )
    elif isinstance(condition, IRParameterRef):
        return (f"parameters.{ir.parameters[condition.parameter_index].name}")
    elif isinstance(condition, IRLiteral):
        return (f"{condition.value}")


def resolve_operator_from_ir(operator : BinaryOp) -> str:
    match operator:
        case BinaryOp.EQ:
            return "=="
        case BinaryOp.NE:
            return "!="
        case BinaryOp.GT:
            return ">"
        case BinaryOp.GE:
            return ">="
        case BinaryOp.LT:
            return "<"
        case BinaryOp.LE:
            return "<="
        case BinaryOp.AND:
            return "&&"
        case BinaryOp.OR:
            return "||"
        case _:
            raise TypeError("Invalid Operator enum.")


INDENT = "    "
def indent(lines: list[str], level: int = 1) -> list[str]:
    prefix = INDENT * level
    return [prefix + line if line else "" for line in lines]