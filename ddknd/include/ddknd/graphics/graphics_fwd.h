#pragma once

#include <memory>

namespace ddknd::graphics
{
    class IRendererBackend;
    struct OpenGLBackendDesc;
    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc);

    class RendererSystem;
    class GraphicsAssetStore;
    class GraphicsAssetLoader;

    namespace asset
    {
        struct ShaderResource;
        struct ModelFileResource;
        struct PrimitiveResource;
        struct ModelRenderResource;
        struct FontResource;
    } // namespace asset

} // namespace ddknd::graphics

namespace ddknd::animation
{
    class AnimationAssetStore;
    namespace types
    {
        struct SkeletonResource;
        struct AnimationClipResource;
    } // namespace animation::types
}