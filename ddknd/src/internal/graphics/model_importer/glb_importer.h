#pragma once

#include <string>
#include <optional>
#include "internal/graphics/model_importer/model_import_types.h"



namespace ddknd::graphics::internal
{
    /**
    * @brief Imports glTF data into an intermediate representation.
    *
    * This stage preserves asset relationships using import-time indices
    * and does not create runtime graphics or animation resources.
    */
    std::optional<::ddknd::graphics::internal::types::ModelImportData> ImportModel(const std::string& path);
}// namespace ddknd::graphcis::internal