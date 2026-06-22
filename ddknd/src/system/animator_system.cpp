#include <ddknd/system/animator_system.h>

#include <ddknd/graphics/gfx_type.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/animation.h>


namespace ddknd::system
{
    void AnimationPlaybackSystem::UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                       ::ddknd::component::PoseComponent& pose,
                       const ::ddknd::animation::types::SkeletonResource& skeleton,
                       const ::ddknd::animation::types::AnimationClipResource& clip, const float dt)
    {
        ::ddknd::animation::AnimatorSystem::UpdateAnimator(skeleton, clip, playback.state, pose.pose, dt);
    }

    void AnimationPlaybackSystem::InitializePose(::ddknd::component::PoseComponent& pose, const ::ddknd::animation::types::SkeletonResource& skeleton)
    {
        ddknd::animation::AnimatorSystem::InitializePose(skeleton, pose.pose);
    }
} // namespace ddknd::system