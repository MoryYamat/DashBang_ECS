#pragma once

namespace ddknd::graphics::internal::types
{
    struct ModelImportData;
}

namespace ddknd::graphics::internal::debug
{
    void DebugImportedMaterials(const ddknd::graphics::internal::types::ModelImportData& out);
    void DebugImportedMaterialLinks(const ddknd::graphics::internal::types::ModelImportData& out);
}