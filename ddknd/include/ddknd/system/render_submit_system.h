#pragma once

#include <ddknd/component/component_fwd.h>
#include <ddknd/graphics/graphics_fwd.h>

namespace ddknd::system
{
    struct SkinnedRenderSubmitSystem
    {
        static void UpdateOne(::ddknd::graphics::RendererSystem& renderer,
                              const ::ddknd::component::SkinnedModelComponent& modelComp,
                              const ::ddknd::component::MaterialComponent& materialComp,
                              const ::ddknd::component::TransformComponent& transformComp,
                              const ::ddknd::component::PoseComponent& poseComp,
                              const ::ddknd::graphics::GraphicsAssetStore& graphicsStore);
    };
} // namespace ddknd::system