from ddknd_fsm.ir import *

class CppGenerator:
    def generate(self, ir: FSMIR) -> str:
        lines: list[str] = []

        lines.append("#pragma once")
        lines.append("")
        lines.append("#include <ddknd/fsm/runtime/definitions.h>")
        lines.append("#include <array>")
        lines.append("#include <cstdint>")
        lines.append("#include <cassert>")
        lines.append("")

        lines.extend(self._generate_state_enum(ir))
        lines.extend("")
        lines.extend(self._generate_parameter_index(ir))
        lines.extend("")
        lines.extend(self._generate_parameters(ir))
        lines.extend("")
        lines.extend(self._generate_FSM_Instance(ir))
        lines.extend("")

        lines.append("")
        lines.extend(self._generate_condition_functions(ir))

        lines.append("")
        lines.extend(self._generate_condition_definition(ir))

        lines.append("")
        lines.extend(self._generate_transitions(ir))

        lines.append("")
        lines.extend(self._generate_definition(ir))
        lines.append("")
        lines.extend(self._generate_traits(ir))

        return "\n".join(lines)

    def _generate_state_enum(self, ir: FSMIR) -> list[str]:
        lines = []

        lines.append(
            f"enum class {ir.name}State : std::uint8_t"
        )
        lines.append("{")

        for state in ir.states:
            lines.append(f"     {state.name},")

        lines.append("};")

        return lines

    def _generate_parameter_index(self, ir : FSMIR) -> list[str]:
        lines = []

        lines.append(
            f"enum class {ir.name}ParameterIndex : std::uint16_t"
        )
        lines.append("{")

        for i, p in enumerate(ir.parameters):
            lines.append(f"     {p.name} = {i},")
        lines.append("};")

        return lines

    def _generate_parameters(self, ir: FSMIR) -> list[str]:
        lines = []

        lines.append(
            f"struct {ir.name}Parameters"
        )
        lines.append("{")

        for parameter in ir.parameters:
            lines.append(f"      {value_type_enum_to_str(parameter.value_type)} {parameter.name}{{}};")

        lines.append("};")

        return lines

    def _generate_FSM_Instance(self, ir: FSMIR) -> list[str]:
        lines = []

        lines.append(
            f"struct {ir.name}Instance"
        )
        lines.append("{")

        lines.append(f"      {ir.name}State current = {ir.name}State::{ir.states[ir.initial_state_index].name};")
        lines.append(f"      {ir.name}State previous = {ir.name}State::{ir.states[ir.initial_state_index].name};")
        lines.append("      std::uint32_t revision = 0;")
        lines.append("};")

        return lines

    def _generate_condition_functions(self, ir :FSMIR) -> list[str]:
        lines = []


        for i, c in enumerate(ir.conditions):
            lines.append(f"static bool {ir.name}Condition{i}(const {ir.name}Parameters& parameters)")
            lines.append("{")
            lines.append(f"     return {generate_condition_expression_from_ir(c, ir)};")
            lines.append("}")

        return lines

    def _generate_condition_definition(self, ir) -> list[str]:
        lines = []

        lines.append(f"using {ir.name}ConditionDefinition = ddknd::fsm::ConditionDefinition<{ir.name}Parameters>;")
        lines.append(f"inline constexpr std::array<{ir.name}ConditionDefinition, {len(ir.conditions)}>")
        lines.append(f"{ir.name}Conditions = ")
        lines.append("{")
        for i, c in enumerate(ir.conditions):
            lines.append(f"     {ir.name}ConditionDefinition{{&{ir.name}Condition{i}}},")
        lines.append("};")

        return lines

    def _generate_transitions(self, ir : FSMIR) -> list[str]:
        lines = []

        lines.append(f"inline constexpr std::array<ddknd::fsm::TransitionDefinition, {len(ir.transitions)}>")
        lines.append(f"{ir.name}Transitions = ")
        lines.append("{")

        for i, t in enumerate(ir.transitions):
            lines.append("  ddknd::fsm::TransitionDefinition{")
            lines.append(f"     .source = static_cast<std::uint32_t>({ir.name}State::{ir.states[t.source_index].name}),")
            lines.append(f"     .destination = static_cast<std::uint32_t>({ir.name}State::{ir.states[t.destination_index].name}),")
            lines.append(f"     .condition = static_cast<std::uint32_t>({t.condition_index}),")
            lines.append(f"     .priority = static_cast<std::uint16_t>({t.priority}),")
            lines.append(f"     .effect = static_cast<std::uint16_t>({t.effect_index}),")
            lines.append("  },")

        lines.append("};")

        return lines

    def _generate_definition(self, ir :FSMIR) -> list[str]:
        lines = []

        lines.append(f"using {ir.name}Definition = ddknd::fsm::FSMDefinition<{ir.name}Parameters>;")
        lines.append(f"{ir.name}Definition {ir.name}Def")
        lines.append("{")
        lines.append(f"     .initialState = static_cast<std::uint32_t>({ir.name}State::{ir.states[ir.initial_state_index].name}),")
        lines.append(f"     .conditions = {ir.name}Conditions,")
        lines.append(f"     .transitions = {ir.name}Transitions")
        lines.append("};")

        return lines

    def _generate_traits(self, ir : FSMIR) -> list[str]:
        lines = []

        lines.append("template<>")
        lines.append(f"struct ddknd::fsm::FSMTraits<{ir.name}Parameters>")
        lines.append("{")
        lines.append(f"      using State = {ir.name}State;")
        lines.append(f"      using Instance = {ir.name}Instance;")
        lines.append(f"      static constexpr auto& Definition()")
        lines.append("      {")
        lines.append(f"          return {ir.name}Def;")
        lines.append("      }")
        lines.append("};")

        return lines

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


def generate_condition_expression_from_ir(condition : IRBinaryExpression, ir : FSMIR) ->list[str]:
    if isinstance(condition, IRBinaryExpression):
        return (
            f"{generate_condition_expression_from_ir(condition.left, ir)} {resolve_operator_from_ir(condition.operator)} {generate_condition_expression_from_ir(condition.right, ir)}"
        )
    elif isinstance(condition, IRParameterRef):
        return (f"parameters.{ir.parameters[condition.parameter_index].name}")
    elif isinstance(condition, IRLiteral):
        return (f"{condition.value}")


def resolve_operator_from_ir(operator : BinaryOp) -> list[str]:
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