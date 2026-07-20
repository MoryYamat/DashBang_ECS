#include <ddknd/system/render_submit_system.h>

#include <ddknd/graphics/gfx_asset_loader.h>
#include <ddknd/graphics/renderer.h>

#include <ddknd/component/gfx_component.h>

namespace
{
    ddknd::graphics::TextureGPURef ResolveTextureGPURef(const ddknd::graphics::types::TextureSlot& slot,
                                                        const ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        ddknd::graphics::TextureGPURef out{};

        out.texCoord = slot.texCoord;

        if (!slot.texture)
            return out;
        const auto* texture = graphicsStore.TryGet(*slot.texture);
        if (!texture)
            return out;

        out.texture = texture->gpuTexture;

        return out;
    }

    ddknd::graphics::NormalTextureGPURef ResolveNormalTextureGPURef(
        const ddknd::graphics::types::NormalTextureSlot& slot, const ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        ddknd::graphics::NormalTextureGPURef out{};
        out.texCoord = slot.texCoord;
        out.scale = slot.scale;

        if (!slot.texture)
            return out;

        const auto* texture = graphicsStore.TryGet(*slot.texture);
        if (!texture)
            return out;

        out.texture = texture->gpuTexture;
        return out;
    }

    ddknd::graphics::OcclusionTextureGPURef ResolveOcclusionTextureGPURef(
        const ddknd::graphics::types::OcclusionTextureSlot& slot,
        const ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        ddknd::graphics::OcclusionTextureGPURef out{};
        out.texCoord = slot.texCoord;
        out.strength = slot.strength;

        if (!slot.texture)
            return out;

        const auto* texture = graphicsStore.TryGet(*slot.texture);
        if (!texture)
            return out;

        out.texture = texture->gpuTexture;
        return out;
    }

    ddknd::graphics::MaterialDrawData BuildMaterialDrawData(const ddknd::graphics::types::MaterialResource& material,
                                                            const ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        ddknd::graphics::MaterialDrawData out{};

        out.baseColorFactor = material.baseColorFactor;
        out.metallicFactor = material.metallicFactor;
        out.roughnessFactor = material.roughnessFactor;
        out.emissiveFactor = material.emissiveFactor;
        out.alphaMode = material.alphaMode;
        out.alphaCutoff = material.alphaCutoff;
        out.doubleSided = material.doubleSided;

        out.baseColorTexture = ResolveTextureGPURef(material.baseColorTexture, graphicsStore);

        out.metallicRoughnessTexture = ResolveTextureGPURef(material.metallicRoughnessTexture, graphicsStore);

        out.normalTexture = ResolveNormalTextureGPURef(material.normalTexture, graphicsStore);

        out.occlusionTexture = ResolveOcclusionTextureGPURef(material.occlusionTexture, graphicsStore);

        out.emissiveTexture = ResolveTextureGPURef(material.emissiveTexture, graphicsStore);

        return out;
    }
} // namespace

namespace ddknd::system
{
    void SkinnedRenderSubmitSystem::UpdateOne(::ddknd::graphics::RendererSystem& renderer,
                                              const ::ddknd::component::SkinnedModelComponent& modelComp,
                                              const ::ddknd::component::MaterialComponent& materialComp,
                                              const ::ddknd::component::TransformComponent& transformComp,
                                              const ::ddknd::component::PoseComponent& poseComp,
                                              const ::ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        using namespace ddknd::graphics;

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
            if (prim.materialIndex >= model->materials.size())
                continue;

            const auto& materialResource = model->materials[prim.materialIndex];

            const auto materialDrawData = BuildMaterialDrawData(materialResource, graphicsStore);

            renderer.Submit(::ddknd::graphics::SkinnedDrawCommand{.mesh = prim.primitive,
                                                                  .shader = shader->program,
                                                                  .modelMatrix = transformComp.worldMatrix,
                                                                  .skinMatrices = poseComp.pose.skinMatrices,
                                                                  .indexCount = prim.indexCount,
                                                                  .material = materialDrawData});
        }
    }

    void MeshRenderSubmitSystem::UpdateOne(ddknd::graphics::RendererSystem& renderer,
                                           const ::ddknd::component::ModelComponent& modelComp,
                                           const ::ddknd::component::MaterialComponent& materialComp,
                                           const ::ddknd::component::TransformComponent& transformComp,
                                           const ::ddknd::graphics::GraphicsAssetStore& graphicsStore)
    {
        using namespace ddknd::graphics;

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
            if (prim.materialIndex >= model->materials.size())
                continue;

            const auto& materialResource = model->materials[prim.materialIndex];

            const auto materialDrawData = BuildMaterialDrawData(materialResource, graphicsStore);

            renderer.Submit(::ddknd::graphics::MeshRenderCommand{.mesh = prim.primitive,
                                                                  .shader = shader->program,
                                                                  .modelMatrix = transformComp.worldMatrix,
                                                                  .indexCount = prim.indexCount,
                                                                  .material = materialDrawData});
        }
    }
} // namespace ddknd::system