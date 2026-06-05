#pragma once

#include "ddknd/asset/asset_fwd.h"
#include <cstdint>

#include "ddknd/graphics/gfx_type.h"
#include "ddknd/math/math.h"

namespace ddknd::component
{
    struct ModelRendererComponent
    {
        // AssetHandle<ModelAssetTag> model;
        // AssetHandle<ShaderAssetTag> shader;
    };

    struct TransformComponent
    {
        private: 
            using TRS = ::ddknd::math::TRS;
            using Mat4f = ::ddknd::math::Mat4f;

        public:
        TRS localTRS;

        Mat4f worldMatrix = Mat4f::Identity();

        bool dirty = true;
    };

    struct AnimatorComponent
    {
        ::ddknd::animation::AnimationState state;
        ::ddknd::animation::Pose pose;
    };
}// namespace ddknd::component