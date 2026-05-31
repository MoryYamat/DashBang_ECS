#pragma once

#include <vector>

namespace ddknd::graphics::internal::types
{
    struct FontGlyphImportData
    {
        float x0, y0, x1, y1;
        float xoff, yoff;
        float xadvance;
    };

    struct FontImportData
    {
        int atlasWidth = 0;
        int atlasHeight = 0;
        int firstCodepoint = 32;
        int glyphCount = 96;
        std::vector<unsigned char> atlasBitmap;
        std::vector<FontGlyphImportData> glyphs;
    };

} // namespace ddknd::graphics::internal::types