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
        std::string scheme;                         // e.g. "res"

        // absolute or executable-relative
        std::filesystem::path mountRoot;//e.g. "assets/..." etc.
    };


    std::unique_ptr<IPathResolver>
	CreateVfsResolver(std::span<const VfsMount> mounts);

}// namespace ddknd::io