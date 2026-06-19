#pragma once

namespace ddknd::animation
{
    namespace types
    {
        struct SkeletonResource;
        struct AnimationClipResource;
        
        struct Pose;
        struct AnimationState;
    } // namespace types

    class AnimatorSystem
    {
      public:
        static void InitializePose(const types::SkeletonResource& skeleton, types::Pose& pose);
        static void UpdateGlobalPose(const types::SkeletonResource& skeleton, types::Pose& pose);
        static void SampleAnimation(const types::SkeletonResource& skeleton, const types::AnimationClipResource& clip,
                                    float time, types::Pose& pose);
        static void UpdateAnimator(const types::SkeletonResource& skeleton, const types::AnimationClipResource& clip,
                                   types::AnimationState& state, types::Pose& pose, float deltaTime);
    };

} // namespace ddknd::animation