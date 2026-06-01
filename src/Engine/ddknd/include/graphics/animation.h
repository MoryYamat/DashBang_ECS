#pragma once

namespace ddknd::animation
{
    namespace types
    {
        struct SkeletonResource;
        struct AnimationClipResource;
    }

    struct Pose;

    class AnimaterSystem
    {
        public:
            static void InitializePose(const types::SkeletonResource& skeleton, Pose& pose);
            static void SampleAnimation(const types::AnimationClipResource& clip, float time, Pose& pose);
    };

}