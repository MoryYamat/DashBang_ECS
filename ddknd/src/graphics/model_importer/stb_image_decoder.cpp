#include "internal/graphics/model_importer/stb_image_decoder.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace ddknd::graphics::internal
{
    std::optional<DecodedImageRGBA8> DecodeImageRGBA8(std::span<const std::uint8_t> encodedBytes) 
    {
        if(encodedBytes.empty())
        {
            return std::nullopt;
        }

        int width = 0;
        int height = 0;
        int sourceChannels = 0;

        stbi_uc* raw = stbi_load_from_memory(
            encodedBytes.data(),
            static_cast<int>(encodedBytes.size()),
            &width,
            &height,
            &sourceChannels,
            4// for RGBA
        );

        if(!raw || width <= 0 || height <= 0)
        {
            if(raw)
                stbi_image_free(raw);
            return std::nullopt;
        }

        const std::size_t byteSize = 
            static_cast<std::size_t>(width) * static_cast<std::size_t>(height) * 4;

        DecodedImageRGBA8 out{};
        out.width = static_cast<std::uint32_t>(width);
        out.height = static_cast<std::uint32_t>(height);
        out.channels = 4;
        out.pixels.assign(raw, raw+byteSize);

        stbi_image_free(raw);

        return out;
    }
} // namespace ddknd::graphics::internal