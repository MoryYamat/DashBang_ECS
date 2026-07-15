#include "ddknd/io/io.h"
#include "internal/io/io.h"

#include "internal/asset/shader_descriptor_parser.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <string_view>

// Resolve the executable directory using the platform-specific API.
#if defined(_WIN32)
#include <windows.h>
#elif defined(__linux__)
#include <iterator>
#include <unistd.h>
#endif

#include <spdlog/spdlog.h>

// helpers
namespace
{
    std::optional<std::pair<std::string_view, std::string_view>> SplitScheme(std::string_view vpath)
    {
        const auto pos = vpath.find("://");
        if (pos == std::string_view::npos)
            return std::nullopt;

        const auto scheme = vpath.substr(0, pos);
        const auto rest = vpath.substr(pos + 3); // skip "://"
        if (scheme.empty() || rest.empty())
            return std::nullopt;

        return std::make_pair(scheme, rest);
    }

    // trim space / return / carrage return
    std::string_view trim(std::string_view s)
    {
        const auto is_space = [](char c) { return c == ' ' || c == '\t' || c == '\r'; };

        while (!s.empty() && is_space(s.front()))
            s.remove_prefix(1);
        while (!s.empty() && is_space(s.back()))
            s.remove_suffix(1);
        return s;
    }

    // find values
    const std::vector<std::string>* find_values(const ddknd::asset::parser::KeyValueDoc& doc, std::string_view key)
    {
        auto it = doc.values.find(std::string(key));
        if (it == doc.values.end())
            return nullptr;
        return &it->second;
    }

    // Resolve the executable directory independently of the current working directory
    std::optional<std::filesystem::path> getExeDir()
    {
#if defined(_WIN32)
        std::vector<wchar_t> buffer(MAX_PATH);
        while (true)
        {
            DWORD length = GetModuleFileNameW(nullptr, buffer.data(), buffer.size());

            if (length == 0)
                return std::nullopt;

            if (length < buffer.size())
                return std::filesystem::path(buffer.data()).parent_path();

            buffer.resize(buffer.size() * 2);
        }
#elif defined(__linux__)
        std::vector<char> buffer(1024);
        while (true)
        {
            const auto length = ::readlink("/proc/self/exe", buffer.data(), buffer.size());

            if (length < 0)
                return std::nullopt;

            if (static_cast<std::size_t>(length) < buffer.size())
            {
                return std::filesystem::path(std::string_view(buffer.data(), static_cast<std::size_t>(length)))
                    .parent_path();
            }

            buffer.resize(buffer.size() * 2);
        }
#else
        std::error_code ec;
        auto cwd = std::filesystem::current_path(ec);
        if (ec)
            return std::nullopt;
        return cwd;

#endif
    }

    /**
     * @brief seraches upward from startDir for relativeRoot.
     *
     * The search stops after maxParentDepth parent levels and returns
     * the canonical path of the first matching directory.
     */
    std::optional<std::filesystem::path> FindMountRootUpward(const std::filesystem::path& startDir,
                                                             const std::filesystem::path& relativeRoot,
                                                             int maxDepth = 6)
    {
        if (relativeRoot.empty() || relativeRoot.is_absolute())
            return std::nullopt;

        auto dir = std::filesystem::absolute(startDir);

        for (int depth = 0; depth <= maxDepth; ++depth)
        {
            // Combine paths
            auto candidate = dir / relativeRoot;

            std::error_code ec;
            if (std::filesystem::exists(candidate) && std::filesystem::is_directory(candidate))
            {
                auto canon = std::filesystem::weakly_canonical(candidate, ec);
                if (!ec)
                    return canon;
            }
            auto parent = dir.parent_path();
            if (parent == dir)
                break;

            dir = parent;
        }

        return std::nullopt;
    }

    bool IsSubPath(const std::filesystem::path& base, const std::filesystem::path& target)
    {
        auto b = base.lexically_normal();
        auto t = target.lexically_normal();

        auto bit = b.begin();
        auto tit = t.begin();

        for (; bit != b.end() && tit != t.end(); ++bit, ++tit)
        {
            if (*bit != *tit)
                return false;
        }

        return bit == b.end();
    }
} // namespace

// private type
namespace
{
    /**
     *
     */
    class MountTable
    {
      public:
        void Mount(std::string scheme, std::filesystem::path root)
        {
            mounts_[std::move(scheme)] = std::move(root);
        }

        void Unmount(std::string scheme)
        {
            mounts_.erase(std::string(scheme));
        }

        const std::filesystem::path* TryGetRoot(std::string_view scheme) const
        {
            auto it = mounts_.find(std::string(scheme));
            if (it == mounts_.end())
                return nullptr;
            return &it->second;
        }

        // for test
        std::unordered_map<std::string, std::filesystem::path>& Get()
        {
            return mounts_;
        }

      private:
        std::unordered_map<std::string, std::filesystem::path> mounts_; // scheme -> abs_path
    };

    class VfsResolver final : public ::ddknd::io::IPathResolver
    {
      public:
        explicit VfsResolver(const MountTable mounts) : mounts_(std::move(mounts)) {}

        // if vpath is abs-path then return it
        std::optional<std::filesystem::path> TryResolve(std::string_view vpath) const override
        {
            const auto parts = SplitScheme(vpath);
            if (!parts)
                return std::nullopt;

            const auto [scheme, rest] = *parts;

            const auto* root = mounts_.TryGetRoot(scheme);
            if (!root)
                return std::nullopt;

            std::filesystem::path rel(rest);
            if (rel.empty() || rel.is_absolute())
                return std::nullopt;

            std::error_code ec;
            const auto resolved = std::filesystem::weakly_canonical((*root) / rel, ec);
            if (ec)
                return std::nullopt;

            if (!IsSubPath(*root, resolved))
                return std::nullopt;

            return resolved;
        }

      private:
        MountTable mounts_;
    };

} // namespace

namespace ddknd::io
{
    std::optional<std::string> ReadAllText(const std::filesystem::path& path)
    {
        std::ifstream ifs(path, std::ios::binary);
        if (!ifs)
        {
            spdlog::error("[ReadAllText]: failed to read file");
            spdlog::error("exists = {}", std::filesystem::exists(path));
            spdlog::error("path = {}", path.string());
            return std::nullopt;
        }

        // Get file size
        ifs.seekg(0, std::ios::end);              // move streaming buffer's reading pos to ios::end
        const std::streamsize size = ifs.tellg(); // get current reading pos
        if (size < 0)
        {
            spdlog::error("[ReadAllText]: unexpected error / invalid read pos ");
            return std::nullopt;
        }

        // TODO: try catch

        std::string buffer;
        buffer.resize(static_cast<std::size_t>(size));

        ifs.seekg(0, std::ios::beg);
        if (!ifs.read(buffer.data(), size))
        {
            spdlog::error("[ReadAllText]: failed to read all buffer data\n");
            return std::nullopt;
        }

        return buffer;
    }

    /**
     * @brief Builds a VFS resolver from the supplied mount definitions.
     *
     * Absolute mount roots are canonicalized directly.
     * Relative mount roots are resolved by searching upward from the
     * executable directory, then stored as canonical absolute paths.
     *
     * Mounts that cannot be resolved are skipped.
     */
    std::unique_ptr<IPathResolver> CreateVfsResolver(std::span<const VfsMount> mounts)
    {
        MountTable table;

        auto exeDir = getExeDir();
        if (!exeDir)
        {
            spdlog::error("[CreateVfsResolver] failed to get exe dir");
            return std::make_unique<VfsResolver>(std::move(table));
        }

        // Resolve every mount root to a canonical absolute path.
        for (const auto& m : mounts)
        {
            if (m.scheme.empty())
                continue;

            std::filesystem::path rootPath(m.mountRoot);
            std::optional<std::filesystem::path> resolvedRoot;

            if (rootPath.is_absolute())
            {
                std::error_code ec;
                const bool exists = std::filesystem::exists(rootPath, ec);
                if (!exists || ec)
                {
                    spdlog::debug("[CreateVfsResolver] invalid absolute mount root: scheme={}, root={}, error={}",
                                 m.scheme, m.mountRoot.string(), ec.message());

                    continue;
                }

                ec.clear();
                const bool isDirectory = std::filesystem::is_directory(rootPath, ec);
                if (ec || !isDirectory)
                {
                    spdlog::debug(
                        "[CreateVfsResolver] absolute mount root is not a directory: scheme={}, root={}, error={}",
                        m.scheme, m.mountRoot.string(), ec.message());

                    continue;
                }

                ec.clear();
                resolvedRoot = std::filesystem::weakly_canonical(rootPath, ec);
                if (ec)
                {
                    spdlog::debug("[CreateVfsResolver] failed to canonicalize mount root: scheme={}, root={}, error={}",
                                 m.scheme, m.mountRoot.string(), ec.message());
                    continue;
                }
            }
            else
            {
                resolvedRoot = FindMountRootUpward(*exeDir, rootPath);
            }

            if (!resolvedRoot)
            {
                spdlog::debug("[CreateVfsResolver] mount root not found: scheme={}, root={}", m.scheme, m.mountRoot.string());
                continue;
            }

            table.Mount(std::string(m.scheme), *resolvedRoot);
            spdlog::debug("[CreateVfsResolver] mounted scheme={} -> {}", m.scheme, resolvedRoot->string());
        }

        return std::make_unique<VfsResolver>(std::move(table));
    }

    std::optional<std::vector<std::uint8_t>> ReadAllBytes(const std::filesystem::path& path)
    {
        std::ifstream ifs(path, std::ios::binary | std::ios::ate);
        if (!ifs)
        {
            spdlog::error("[ReadAllBytes]: failed to open file");
            spdlog::error("exists = {}", std::filesystem::exists(path));
            spdlog::error("path = {}", path.string());
            return std::nullopt;
        }

        const std::streamsize size = ifs.tellg();

        if (size <= 0)
        {
            spdlog::error("[ReadAllBytes]: invalid file size");
            return std::nullopt;
        }

        std::vector<std::uint8_t> buffer(static_cast<std::size_t>(size));

        ifs.seekg(0, std::ios::beg);
        if (!ifs.read(reinterpret_cast<char*>(buffer.data()), size))
        {
            spdlog::error("[ReadAllBytes]: failed to read file");
            return std::nullopt;
        }

        return buffer;
    }
} // namespace ddknd::io