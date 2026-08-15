from ddknd_fsm.ir import *

class CppGenerator:
    def generate(self, ir: FSMIR) -> str:
        lines: list[str] = []

        lines.append("#pragma once")
        lines.append("")
        lines.append("#include <cstdint>")
        lines.append("")

        lines.extend(self._generate_state_enum(ir))
        lines.append("")
        lines.extend(self._generate_parameters(ir))
        lines.extend("")
        lines.extend(self._generate_FSM_Instance(ir))
        lines.extend("")

        print(lines[18])
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

        lines.append(f"      {ir.name}State current = {ir.name}State::{ir.states[ir.initial_state_index]};")
        lines.append(f"      {ir.name}State previous = {ir.name}State::{ir.states[ir.initial_state_index]};")
        lines.append("      std::uint32_t revision = 0;")
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