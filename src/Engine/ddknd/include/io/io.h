#pragma once

#include <span>
#include <memory>
#include <filesystem>
#include <optional>

namespace ddknd::io
{
    class IPathResolver
    {
        public:
            virtual ~IPathResolver() = default ;
            virtual std::optional<std::filesystem::path>
                TryResolve(std::string_view vpath) const = 0;
    };


    struct VfsMount
    {
        std::string_view scheme;    // "res"
        std::string_view root;      // "assets/" etc.
    };


    std::unique_ptr<IPathResolver>
	CreateVfsResolver(std::span<const VfsMount> mounts);

}// namespace ddknd::io