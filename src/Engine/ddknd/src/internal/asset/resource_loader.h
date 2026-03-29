#pragma once
#include "asset/asset.h"
#include <memory>
#include <string_view>


namespace ddknd::asset::loader
{
    template <typename Resource>
    struct ResourceLoader;

    template <>
    struct ResourceLoader<ddknd::asset::type::ShaderResource>
    {
        static std::unique_ptr<ddknd::asset::type::ShaderResource> Load(const ddknd::io::IPathResolver& vfs,
                                                           ddknd::graphics::IRendererBackend& backend,
                                                           std::string_view vpath);
    };

    template <>
    struct ResourceLoader<ddknd::asset::type::MeshResource>
    {
        static std::unique_ptr<ddknd::asset::type::MeshResource> Load(const ddknd::io::IPathResolver& vfs,
                                                         ddknd::graphics::IRendererBackend& backend,
                                                         std::string_view vpath);
    };
} // namespace ddknd::asset::loader