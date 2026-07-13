#pragma once

#include <memory>

namespace ddknd::graphics
{
    class IRendererBackend;
    struct OpenGLBackendDesc;
    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc);

    class RendererSystem;
    class DebugDrawList;
    struct RenderCamera;

    class GraphicsAssetStore;
    class GraphicsAssetLoader;

} // namespace ddknd::graphics

namespace ddknd::graphics::asset
{
    struct ShaderResource;
    struct ModelFileResource;
    struct PrimitiveResource;
    struct ModelRenderResource;
    struct FontResource;
}// namespace ddknd::graphics::asset

namespace ddknd::animation
{
    class AnimationAssetStore;
}// namespace ddknd::animation

namespace ddknd::animation::types
{
    struct AnimationClipResource;
    struct SkeletonResource;
    struct Pose;
}// namespace ddknd::animation::types