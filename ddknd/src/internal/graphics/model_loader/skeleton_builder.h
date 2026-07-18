#pragma once

#include <unordered_map>
#include "internal/graphics/model_importer/model_import_types.h"

namespace ddknd::graphics::internal
{
    struct SkeletonBuildResult
    {
        ddknd::animation::types::SkeletonResource skeleton;
        std::unordered_map<int, int> nodeToBone;
    };

    SkeletonBuildResult BuildModelSkeletonResource(
        const ddknd::graphics::internal::types::ModelImportData& import,
        int skinIndex
    );
}