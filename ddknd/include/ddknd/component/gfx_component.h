#pragma once

#include "ddknd/graphics/type/animation_types.h"
#include "ddknd/math/math.h"

namespace ddknd::component
{
    struct TransformComponent
    {
      private:
        using TRS = ::ddknd::math::TRS;
        using Mat4f = ::ddknd::math::Mat4f;

      public:
        TRS localTRS;

        Mat4f worldMatrix = Mat4f::Identity();

        bool dirty = true;
    };

    struct AnimationPlaybackComponent
    {
        ::ddknd::animation::types::AnimationState state;
    };

    struct PoseComponent
    {
        ::ddknd::animation::types::Pose pose;
    };

    enum class ProjectionType
    {
        Perspective,
        Orthographic
    };

    struct MainCameraTag
    {
    };

    struct CameraLookComponent
    {
        math::Vec3f forward{0.0f, 0.0f, -1.0f};
        math::Vec3f up{0.0f, 1.0f, 0.0f};
    };

    struct CameraProjectionComponent
    {
        ProjectionType projectionType = ProjectionType::Perspective;

        float fovYRad = math::degToRadf(60.0f);
        float orthoHeight = 10.0f;

        float aspect = 16.0f / 9.0f;
        float nearZ = 0.1f;
        float farZ = 1000.0f;

        bool primary = false;
    };

    struct CameraMatricesComponent
    {
        math::Mat4f view;
        math::Mat4f proj;
        math::Mat4f viewProj;
    };

    struct SkinnedModelComponent
    {
        asset::AssetID<asset::tag::Model> model;
    };

    struct MaterialComponent
    {
        asset::AssetID<asset::tag::Shader> shader;
    };

} // namespace ddknd::component