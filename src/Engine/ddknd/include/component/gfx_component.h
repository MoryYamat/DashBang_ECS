#pragma once

#include "asset/asset_fwd.h"
#include <cstdint>

#include "graphics/gfx_type.h"

namespace ddknd::component
{
    struct ModelRendererComponent
    {
        // AssetHandle<ModelAssetTag> model;
        // AssetHandle<ShaderAssetTag> shader;
    };

    struct AnimatorComponent
    {
        ::ddknd::animation::AnimationState states;
        ::ddknd::animation::Pose pose;
    };
}// namespace ddknd::component