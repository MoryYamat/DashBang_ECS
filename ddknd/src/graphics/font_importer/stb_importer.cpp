#include "internal/graphics/font_importer/stb_font_importer.h"

#include "internal/io/io.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

namespace ddknd::graphics::internal
{
    std::optional<ddknd::graphics::internal::types::FontImportData> ImportFont(const std::string& path, float pixelHeight)
    {
        constexpr int atlasWidth = 512;
        constexpr int atlasHeight = 512;
        constexpr int firstCodepoint = 32;
        constexpr int glyphCount = 96;

        auto fontBytes = io::ReadAllBytes(path);
        if(!fontBytes)
            return std::nullopt;

        internal::types::FontImportData out;
        out.atlasWidth = atlasWidth;
        out.atlasHeight = atlasHeight;
        out.firstCodepoint = firstCodepoint;
        out.glyphCount = glyphCount;
        out.atlasBitmap.resize(atlasWidth * atlasHeight);
        out.glyphs.resize(glyphCount);

        std::vector<stbtt_bakedchar> bakedChars(glyphCount);

        const int result = stbtt_BakeFontBitmap(
            reinterpret_cast<const unsigned char*>(fontBytes->data()),
            0,
            pixelHeight,
            out.atlasBitmap.data(),
            atlasWidth,
            atlasHeight,
            firstCodepoint,
            glyphCount,
            bakedChars.data()
        );

        if(result <= 0)
        {
            return std::nullopt;
        }

        for(int i = 0; i < glyphCount; i++)
        {
            const auto& src = bakedChars[i];

            auto& dst = out.glyphs[i];
            dst.x0 = static_cast<float>(src.x0);
            dst.y0 = static_cast<float>(src.y0);
            dst.x1 = static_cast<float>(src.x1);
            dst.y1 = static_cast<float>(src.y1);
            dst.xoff = src.xoff;
            dst.yoff = src.yoff;
            dst.xadvance = src.xadvance;
        }

        return out;
    }
}