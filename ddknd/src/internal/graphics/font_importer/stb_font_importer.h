#pragma once

#include <optional>
#include <string>

#include "internal/graphics/font_importer/font_import_types.h"


namespace ddknd::graphics::internal
{
    std::optional<ddknd::graphics::internal::types::FontImportData> ImportFont(const std::string& path, float pixelHeight);
}