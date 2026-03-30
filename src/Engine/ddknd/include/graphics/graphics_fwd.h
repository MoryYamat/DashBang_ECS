#pragma once

#include <memory>

namespace ddknd::graphics
{
    class IRendererBackend;
    struct OpenGLBackendDesc;
    std::unique_ptr<IRendererBackend> CreateOpenGLBackend(const OpenGLBackendDesc& desc);

    class RendererSystem;
}// namespace ddknd::graphics