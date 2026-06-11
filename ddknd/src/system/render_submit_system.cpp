#include <ddknd/system/render_submit_system.h>

#include <ddknd/graphics/gfx_asset_loader.h>
#include <ddknd/graphics/renderer.h>

#include <ddknd/component/gfx_component.h>

namespace ddknd::system
{
    void SkinnedRenderSubmitSystem::UpdateOne(::ddknd::graphics::RendererSystem& renderer,
                                              const ::ddknd::component::SkinnedModelComponent& modelComp,
                                              const ::ddknd::component::MaterialComponent& materialComp,
                                              const ::ddknd::component::TransformComponent& transformComp,
                                              const ::ddknd::component::PoseComponent& poseComp,
                                              const ::ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        const auto* model = graphicsStore.TryGet(modelComp.model);
        const auto* shader = graphicsStore.TryGet(materialComp.shader);

        if (!model || !shader)
        {
            return;
        }

        const auto modelT = ::ddknd::math::ExtractTranslation(transformComp.worldMatrix);

        // std::cerr << "submit modelMatrix translation" << modelT << "\n";
        // std::cerr << "localTRS translation = "
        //   << transformComp.localTRS.translation << "\n";

        for (const auto& prim : model->primitives)
        {
            renderer.Submit(::ddknd::graphics::SkinnedDrawCommand{.mesh = prim.prim,
                                                                  .shader = shader->program,
                                                                  .modelMatrix = transformComp.worldMatrix,
                                                                  .skinMatrices = poseComp.pose.skinMatrices,
                                                                  .indexCount = prim.indexCount});
        }
    }
} // namespace ddknd::system