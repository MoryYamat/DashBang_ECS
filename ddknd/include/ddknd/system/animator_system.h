#pragma once

#include <ddknd/component/component_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

namespace ddknd::system
{
    class AnimationPlaybackSystem
    {
      public:
        // writes:  AnimationPlaybackComponent 
        //          PoseComponent
        // reads:   SkinnedModelComponent
        //          GraphicsAssetStore
        //          AnimationClipResource
        static void UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                       ::ddknd::component::PoseComponent& pose,
                       const ::ddknd::animation::types::SkeletonResource& skeleton,
                       const ::ddknd::animation::types::AnimationClipResource& clip, const float dt);

      private:
    };
} // namespace ddknd::system