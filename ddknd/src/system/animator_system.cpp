#include <ddknd/system/animator_system.h>

#include <ddknd/graphics/gfx_type.h>

#include <ddknd/component/gfx_component.h>
#include <ddknd/graphics/animation.h>

namespace ddknd::system
{
    void AnimationPlaybackSystem::UpdateOne(::ddknd::component::AnimationPlaybackComponent& playback,
                                            const ::ddknd::animation::types::AnimationClipResource& clip,
                                            const float dt)
    {
        ::ddknd::animation::AnimatorSystem::UpdateAnimationState(playback.state, clip, dt);
    }

    void AnimationPoseSamplingSystem::UpdateOne(::ddknd::component::PoseComponent& pose,
                                                const ::ddknd::component::AnimationPlaybackComponent& playback,
                                                const ::ddknd::animation::types::SkeletonResource& skeleton,
                                                const ::ddknd::animation::types::AnimationClipResource& clip)
    {
        ddknd::animation::AnimatorSystem::UpdatePose(pose.pose, playback.state, skeleton, clip);
    }

    void AnimationPoseSamplingSystem::InitializePose(::ddknd::component::PoseComponent& pose,
                                                     const ::ddknd::animation::types::SkeletonResource& skeleton)
    {
        ddknd::animation::AnimatorSystem::InitializePose(pose.pose, skeleton);
    }
} // namespace ddknd::system