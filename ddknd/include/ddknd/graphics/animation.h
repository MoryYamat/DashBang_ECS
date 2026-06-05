#pragma once

namespace ddknd::animation
{
    namespace types
    {
        struct SkeletonResource;
        struct AnimationClipResource;
    } // namespace types

    struct Pose;
    struct AnimationState;
    class AnimatorSystem
    {
      public:
        static void InitializePose(const types::SkeletonResource& skeleton, Pose& pose);
        static void UpdateGlobalPose(const types::SkeletonResource& skeleton, Pose& pose);
        static void SampleAnimation(const types::SkeletonResource& skeleton, const types::AnimationClipResource& clip,
                                    float time, Pose& pose);
        static void UpdateAnimator(const types::SkeletonResource& skeleton, const types::AnimationClipResource& clip,
                                   AnimationState& state, Pose& pose, float deltaTime);
    };

} // namespace ddknd::animation