#include "support/test_check.h"

#include <ddknd/graphics/type/animation_types.h>
#include <ddknd/graphics/animation.h>
#include <ddknd/math/math.h>

#include <cmath>
#include <cstdlib>

namespace
{
    using ddknd::animation::AnimatorSystem;
    using ddknd::animation::types::Pose;
    using ddknd::animation::types::SkeletonResource;
    using ddknd::math::Mat4f;
    using ddknd::math::TRS;
    using ddknd::math::Vec3f;

    bool NearlyEqual(float lhs, float rhs)
    {
        return std::abs(lhs - rhs) <= ddknd::math::kEpsilon<float>;
    }
    
    bool NearlyEqual(const Mat4f& lhs, const Mat4f& rhs)
    {
        for (std::size_t row = 0; row < 4; ++row)
        {
            for (std::size_t column = 0; column < 4; ++column)
            {
                if (!NearlyEqual(lhs(row, column), rhs(row, column)))
                {
                    return false;
                }
            }
        }

        return true;
    }


    TRS MakeTranslation(float x, float y, float z)
    {
        TRS trs{};
        trs.translation = Vec3f{x, y, z};
        return trs;
    }

    void TestGlobalPoseWithNonJointParentCorrection()
    {
        SkeletonResource skeleton;
        skeleton.bones.resize(2);

        // Root joint
        skeleton.bones[0].parent = -1;
        skeleton.bones[0].bindLocalTRS = MakeTranslation(1.0f,0.0f,0.0f);
        skeleton.bones[0].parentCorrection = Mat4f::Identity();
        skeleton.bones[0].inverseBindMatrix = Mat4f::Identity();

        // Child joint
        skeleton.bones[1].parent = 0;
        skeleton.bones[1].bindLocalTRS = MakeTranslation(0.0f,0.0f,3.0f);
        skeleton.bones[1].inverseBindMatrix = Mat4f::Identity();

        // A non-joint node between the root joint and child joint.
        const TRS nonJointNode = MakeTranslation(0.0f, 2.0f, 0.0f);
        skeleton.bones[1].parentCorrection = nonJointNode.ToMatrix();

        Pose pose;
        AnimatorSystem::InitializePose(pose,skeleton);

        const Mat4f expectedRoot = skeleton.bones[0].bindLocalTRS.ToMatrix();
        const Mat4f expectedChildLocal = skeleton.bones[1].parentCorrection * skeleton.bones[1].bindLocalTRS.ToMatrix();
        const Mat4f expectedChildGlobal = expectedRoot * expectedChildLocal;

        TEST_CHECK(pose.globalMatrices.size() == 2);
        TEST_CHECK(pose.globalMatrices.size() == 2);

        TEST_CHECK(NearlyEqual(pose.globalMatrices[0], expectedRoot));
        TEST_CHECK(NearlyEqual(pose.localMatrices[1],expectedChildLocal));
        TEST_CHECK(NearlyEqual(pose.globalMatrices[1], expectedChildGlobal));
    }

} // namespace

int main()
{
    TestGlobalPoseWithNonJointParentCorrection();

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}