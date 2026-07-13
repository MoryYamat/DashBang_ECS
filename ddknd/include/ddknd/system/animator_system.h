#pragma once

#include <ddknd/component/component_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

namespace ddknd::system
{
    class AnimationPlaybackSystem
    {
      public:
        static void UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                              const ::ddknd::animation::types::AnimationClipResource& clip, const float dt);
    };

    class AnimationPoseSamplingSystem
    {
      public:
        static void UpdateOne(::ddknd::component::PoseComponent& pose,
                              const ::ddknd::component::AnimationPlaybackComponent& playback,
                              const ::ddknd::animation::types::SkeletonResource& skeleton,
                              const ::ddknd::animation::types::AnimationClipResource& clip);

        static void InitializePose(::ddknd::component::PoseComponent& pose,
                                   const ::ddknd::animation::types::SkeletonResource& skeleton);
    };
} // namespace ddknd::system