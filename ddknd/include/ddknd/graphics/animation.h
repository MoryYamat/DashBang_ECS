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
        static void InitializePose(types::Pose& pose, const types::SkeletonResource& skeleton);

        static void UpdateAnimationState(types::AnimationState& state, 
                                         const types::AnimationClipResource& clip,
                                         const float deltaTime);
        static void UpdatePose(types::Pose& pose, 
                               const types::AnimationState& state,
                               const types::SkeletonResource& skeleton, 
                               const types::AnimationClipResource& clip
                               );

      private:
        static void UpdateGlobalPose(types::Pose& pose, const types::SkeletonResource& skeleton);
        static void SampleAnimation(types::Pose& pose, 
                                    const types::SkeletonResource& skeleton,
                                    const types::AnimationClipResource& clip,
                                    const float time);
    };

} // namespace ddknd::animation