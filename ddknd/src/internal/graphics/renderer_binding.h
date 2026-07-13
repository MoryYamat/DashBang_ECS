#pragma once

#include <cstdint>

namespace ddknd::graphics::internal::binding
{
    // Fixed texture-unit bindings used by shader samplers.
    constexpr std::uint32_t BaseColorTexture = 0;
    constexpr std::uint32_t MetallicRoughnessTexture = 1;
    constexpr std::uint32_t NormalTexture = 2;
    constexpr std::uint32_t OcclusionTexture = 3;
    constexpr std::uint32_t EmissiveTexture = 4;
} // namespace ddknd::graphics::internal::binding