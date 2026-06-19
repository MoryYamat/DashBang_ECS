#pragma once

#include <cstdint>
#include <vector>
#include <span>
#include <optional>

namespace ddknd::graphics::internal
{
    struct DecodedImageRGBA8
    {
        std::vector<std::uint8_t> pixels;
        std::uint32_t width = 0;
        std::uint32_t height = 0;
        std::uint32_t channels = 4;
    };

    std::optional<DecodedImageRGBA8> DecodeImageRGBA8(std::span<const std::uint8_t> encodedBytes);
} // namespace ddknd::graphics::internal