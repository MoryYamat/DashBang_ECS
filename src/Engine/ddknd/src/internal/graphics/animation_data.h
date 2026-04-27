#pragma once

#include <vector>
#include "model_data.h"


namespace ddknd::animation::asset
{
    // struct SkeletonResource
    // {
    //     ::ddknd::graphics::type::Skeleton skeleton;
    // };

    // struct AnimationClipResource
    // {
    //     ::ddknd::graphics::type::AnimationClip clip;
    // };

    struct ModelAnimationResource
    {
        ::ddknd::graphics::type::Skeleton skeleton;
        std::vector<::ddknd::graphics::type::AnimationClip> clips;
    };
}//namespace ddknd::animation::asset