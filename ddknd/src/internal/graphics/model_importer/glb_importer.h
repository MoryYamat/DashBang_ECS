#pragma once

#include <string>
#include <optional>
#include "internal/graphics/model_importer/model_import_types.h"



namespace ddknd::graphics::internal
{
    // @TODO: dependency
    std::optional<::ddknd::graphics::internal::types::ModelImportData> ImportModel(const std::string& path);
}// namespace ddknd::graphcis::internal