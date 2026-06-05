#pragma once

// fwd
namespace ddknd::animation
{

    namespace types
    {
        struct SkeletonResource;
    }

    struct Pose;
} // namespace ddknd::graphics

namespace ddknd::graphics
{
    class DebugDrawList;
}

namespace ddknd::animation::debug
{
    void TestAnimatorSystemInit(const animation::types::SkeletonResource& skeleton, animation::Pose& pose);
    void TestAnimatorSystemUpdate(const animation::types::SkeletonResource& skeleton, animation::Pose& pose, graphics::DebugDrawList& draw);
} // namespace ddknd::graphics::debug