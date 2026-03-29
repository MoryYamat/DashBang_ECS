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
        std::string scheme;    // "res"
        std::filesystem::path root;      // "assets/" etc.
    };


    std::unique_ptr<IPathResolver>
	CreateVfsResolver(std::span<const VfsMount> mounts);


    // MVP
    class PathResolverExp
    {
        public:
        explicit PathResolverExp(std::span<const VfsMount> mounts);
        
        ~PathResolverExp();

        void Print();

        private:
        struct Impl;
        std::unique_ptr<Impl> impl_;

    };
}// namespace ddknd::io