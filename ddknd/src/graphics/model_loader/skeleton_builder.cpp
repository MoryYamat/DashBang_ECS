#include "internal/graphics/model_loader/skeleton_builder.h"

#include "internal/graphics/model_importer/model_import_types.h"

#include <ddknd/graphics/type/animation_types.h>
#include <ddknd/math/math.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cstddef>
#include <vector>

using ImportSkin = ddknd::graphics::internal::types::ImportSkin;
using ImportModelData = ddknd::graphics::internal::types::ModelImportData;

using Bone = ddknd::animation::types::Bone;

namespace
{
    // Skeleton conversion
    int FindBoneIndexFromNode(const ImportSkin& skin, int nodeIndex)
    {
        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            if (skin.jointNodes[i] == nodeIndex)
                return static_cast<int>(i);
        }

        return -1;
    }

    // Find parent Bone index and accumulate non-joint nodes in Bone::parentCorrection.
    int FindNearestParentBoneIndex(const ImportModelData& import, const ImportSkin& skin, int parentNode,
                                   ddknd::math::Mat4f& parentCorrection)
    {
        parentCorrection = ddknd::math::Mat4f::Identity();

        std::vector<int> nonJointParents;

        int n = parentNode;

        while (n >= 0)
        {
            const int boneIndex = FindBoneIndexFromNode(skin, n);

            if (boneIndex >= 0)
            {
                std::reverse(nonJointParents.begin(), nonJointParents.end());

                for (int nodeIndex : nonJointParents)
                {
                    parentCorrection = parentCorrection * import.nodes[nodeIndex].localMatrix;
                }

                return boneIndex;
            }

            nonJointParents.push_back(n);
            n = import.nodes[n].parent;
        }

        std::reverse(nonJointParents.begin(), nonJointParents.end());

        for (int nodeIndex : nonJointParents)
        {
            parentCorrection = parentCorrection * import.nodes[nodeIndex].localMatrix;
        }

        return -1;
    }

    enum class VisitState : std::uint8_t
    {
        Unvisited,
        Visiting,// cyclic detection
        Visited
    };

    bool VisitBone(int boneIndex, const std::vector<Bone>& bones, std::vector<VisitState>& visited,
                   std::vector<std::uint32_t>& evaluationOrder)
    {
        if(visited[boneIndex] == VisitState::Visited)
        {
            return true;
        }

        if(visited[boneIndex] == VisitState::Visiting)
        {
            // cycle detected
            return false;
        }

        visited[boneIndex] = VisitState::Visiting;

        const int parent = bones[boneIndex].parent;
        if(parent >= 0)
        {
            if(!VisitBone(parent, bones, visited, evaluationOrder))
            {
                return false;
            }
        }

        visited[boneIndex] = VisitState::Visited;
        evaluationOrder.push_back(boneIndex);

        return true;
    }
} // namespace

namespace ddknd::graphics::internal
{

    /**
     * Build the runtime skeleton in glTF skin.jointNodes order.
     *
     * Transforms from intervening non-joint nodes are accumulated into
     * Bone::parentCorrection.
     */
    SkeletonBuildResult BuildModelSkeletonResource(const ImportModelData& import, int skinIndex)
    {
        const auto& skin = import.skins[skinIndex];
        SkeletonBuildResult result;
        auto& skeletonResult = result.skeleton;
        skeletonResult.bones.resize(skin.jointNodes.size());

        // spdlog::info("jointNodes");

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            const int nodeIndex = skin.jointNodes[i];
            result.nodeToBone[nodeIndex] = static_cast<int>(i); // (node, bone)
            // spdlog::info("nodeIndex={}, boneIndex={}", nodeIndex, i);
        }

        for (std::size_t i = 0; i < skin.jointNodes.size(); ++i)
        {
            const int nodeIndex = skin.jointNodes[i];
            const auto& node = import.nodes[nodeIndex];

            Bone b{};

            ddknd::math::Mat4f parentCorrection = ddknd::math::Mat4f::Identity();

            b.parent = FindNearestParentBoneIndex(import, skin, node.parent, parentCorrection);

            b.parentCorrection = parentCorrection;

            if (i < skin.inverseBindMatrices.size())
            {
                b.inverseBindMatrix = skin.inverseBindMatrices[i];
            }
            else
            {
                b.inverseBindMatrix = ddknd::math::Mat4f::Identity();
            }

            b.bindLocalTRS = node.localTRS;
            b.bindLocalMatrix = node.localMatrix;

            skeletonResult.bones[i] = b;
        }

        skeletonResult.evaluationOrder.clear();
        skeletonResult.evaluationOrder.reserve(skeletonResult.bones.size());
        std::vector<VisitState> visited(skeletonResult.bones.size(), VisitState::Unvisited);
        for (std::size_t i = 0; i < skeletonResult.bones.size(); i++)
        {
            if (!VisitBone(static_cast<int>(i), skeletonResult.bones, visited, skeletonResult.evaluationOrder))
            {
                // invalid skeleton
            }
        }

        assert(skeletonResult.evaluationOrder.size() == skeletonResult.bones.size());

        return result;
    }

} // namespace ddknd::graphics::internal