#include <internal/graphics/model_loader/skeleton_builder.h>

#include <internal/graphics/model_importer/model_import_types.h>

#include <ddknd/math/math.h>
#include <ddknd/graphics/animation.h>
#include <ddknd/graphics/type/gfx_resource_types.h>

#include "support/test_check.h"

namespace
{
    using Vec3f = ddknd::math::Vec3f;
    using Mat4f = ddknd::math::Mat4f;
    using TRS = ddknd::math::TRS;

    using ImportModelData = ddknd::graphics::internal::types::ModelImportData;
    using ImportNode = ddknd::graphics::internal::types::ImportNode;
    using ImportSkin = ddknd::graphics::internal::types::ImportSkin;

    using SkeletonBuildResult = ddknd::graphics::internal::SkeletonBuildResult;

    using ModelRenderResource = ddknd::graphics::types::ModelRenderResource;

    using AnimatorSystem = ddknd::animation::AnimatorSystem;
    using Pose = ddknd::animation::types::Pose;

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

    void TestBuildSkeletonResourceWithNonJointParent()
    {
        ImportModelData importModel{};

        auto& importNodes = importModel.nodes;

        importNodes.resize(3);

        // Root Node
        auto& rootNode = importNodes[0];
        rootNode.parent = -1;
        rootNode.children.resize(1);
        rootNode.children[0] = 1;
        rootNode.localTRS = MakeTranslation(0.0f, 0.0f, 0.0f);
        rootNode.localMatrix = rootNode.localTRS.ToMatrix();

        // non-joint child node
        auto& childNode = importNodes[1];
        childNode.parent = 0;
        childNode.children.resize(1);
        childNode.children[0] = 2;
        childNode.localTRS = MakeTranslation(0.0f, 2.0f, 0.0f);
        childNode.localMatrix = childNode.localTRS.ToMatrix();
        
        // joint grandchild node
        auto& grandchildNode = importNodes[2];
        grandchildNode.parent = 1;
        grandchildNode.localTRS = MakeTranslation(0.0f, 1.0f, 2.0f);
        grandchildNode.localMatrix = grandchildNode.localTRS.ToMatrix();

        auto& importSkins = importModel.skins;
        importSkins.resize(1);
        auto& skin = importSkins[0];

        skin.skeletonRootNode = 0;
        skin.jointNodes = {0, 2};

        skin.inverseBindMatrices = { Mat4f::Identity(), Mat4f::Identity()};

        const auto result = ddknd::graphics::internal::BuildModelSkeletonResource(importModel, 0);

        TEST_CHECK(result.skeleton.bones.size() == 2);
        TEST_CHECK(result.skeleton.bones[0].parent == -1);
        TEST_CHECK(result.skeleton.bones[1].parent == 0);

        TEST_CHECK(NearlyEqual(result.skeleton.bones[0].inverseBindMatrix, Mat4f::Identity()));
        TEST_CHECK(NearlyEqual(result.skeleton.bones[0].parentCorrection, Mat4f::Identity()));

        TEST_CHECK(NearlyEqual(result.skeleton.bones[1].inverseBindMatrix, Mat4f::Identity()));
        TEST_CHECK(NearlyEqual(result.skeleton.bones[1].parentCorrection, childNode.localMatrix));
    }

}
int main()
{
    TestBuildSkeletonResourceWithNonJointParent();

    return ddknd::tests::failures == 0 ? EXIT_SUCCESS : EXIT_FAILURE;
}