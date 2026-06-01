#include "graphics/debug_animation.h"
#include "graphics/animation.h"
#include "graphics/gfx_type.h"

#include "graphics/renderer.h"
#include "math/math.h"

namespace ddknd::animation::debug
{
    void TestAnimatorSystemInit(const types::SkeletonResource& skeleton, animation::Pose& pose)
    {
        ::ddknd::animation::AnimaterSystem::InitializePose(skeleton, pose);
    }

    void TestAnimatorSystemUpdate(const animation::types::SkeletonResource& skeleton, animation::Pose& pose,
                                  graphics::DebugDrawList& draw)
    {
        using namespace math;
        for (std::size_t i = 0; i < skeleton.bones.size(); i++)
        {
            const int parent = skeleton.bones[i].parent;
            if (parent < 0)
                continue;
            const auto a = ExtractTranslation(pose.globalMatrices[parent]);
            const auto b = ExtractTranslation(pose.globalMatrices[i]);

            math::Vec4f color{0.0f, 1.0f, 1.0f, 1.0f};
            draw.Line(a, b, color);
        }
    }
} // namespace ddknd::animation::debug