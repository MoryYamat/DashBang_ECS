#include "io/io.h"
#include "internal/io/io.h"

#include "internal/asset/shader_descriptor_parser.h"

#include <span>
#include <string_view>
#include <string>
#include <memory>
#include <optional>
#include <filesystem>
#include <iostream>
#include <fstream>

#include <spdlog/spdlog.h>

// utils
namespace
{
	std::optional<std::pair<std::string_view, std::string_view>>
		SplitScheme(std::string_view vpath)
	{
		const auto pos = vpath.find("://");
		if (pos == std::string_view::npos)
			return std::nullopt;

		const auto scheme = vpath.substr(0, pos);
		const auto rest = vpath.substr(pos + 3);		// skip "://"
		if (scheme.empty() || rest.empty())
			return std::nullopt;

		return std::make_pair(scheme, rest);
	}

	// trim space / return / carrage return 
	std::string_view trim(std::string_view s)
	{
		const auto is_space = [](char c)
			{
				return c == ' ' || c == '\t' || c == '\r';
			};

		while (!s.empty() && is_space(s.front())) s.remove_prefix(1);
		while (!s.empty() && is_space(s.back())) s.remove_suffix(1);
		return s;
	}

	// find values
	const std::vector<std::string>* find_values(const ddknd::asset::parser::KeyValueDoc& doc, std::string_view key)
	{
		auto it = doc.values.find(std::string(key));
		if (it == doc.values.end()) return nullptr;
		return &it->second;
	}

}// namespace

// private type
namespace
{
	struct VPath
	{
		std::filesystem::path scheme;
		std::filesystem::path path;
	};

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

		std::optional<std::filesystem::path> TryGetRoot(std::string_view scheme) const
		{
			auto it = mounts_.find(std::string(scheme));
			if (it == mounts_.end())
				return std::nullopt;
			return it->second;
		}


	private:
		std::unordered_map<std::string, std::filesystem::path> mounts_;
	};



	class VfsResolver final : public ::ddknd::io::IPathResolver
	{
	public:
		explicit VfsResolver(const MountTable& mounts) : mounts_(mounts) {}

		// if vpath is abs path then return it
		std::optional<std::filesystem::path> TryResolve(std::string_view vpath) const override
		{
			// OS abs path
			std::filesystem::path p(vpath);
			if (p.is_absolute())
				return std::filesystem::weakly_canonical(p);

			// scheme:// -> resolve vpath with mount
			const auto parts = SplitScheme(vpath);
			if (!parts)
				return std::nullopt;

			const auto [scheme, rest] = *parts;
			const auto rootOpt = mounts_.TryGetRoot(scheme);
			if (!rootOpt)
				return std::nullopt;

			std::filesystem::path restP(rest);
			if (restP.is_absolute())
				return std::nullopt;

			std::filesystem::path abs = (*rootOpt) / restP;		// unit 
			return std::filesystem::weakly_canonical(abs);

		}

	private:
		MountTable mounts_;

	};



}// namespace


namespace ddknd::io
{
    std::optional<std::string>
    ReadAllText(const std::filesystem::path& path)
    {
		std::ifstream ifs(path, std::ios::binary);
		if (!ifs)
		{
			spdlog::error("[ReadAllText]: failed to read file");
			spdlog::error("exists = {}", std::filesystem::exists(path));
			spdlog::error("path = {}", path.string());
			return std::nullopt;
		}
			
		// ファイルサイズ取得
		ifs.seekg(0, std::ios::end);// move streaming buffer's reading pos to ios::end 
		const std::streamsize size = ifs.tellg();// get current reading pos
		if (size < 0)
		{
			spdlog::error("[ReadAllText]: unexpected error / invalid read pos ");
			return std::nullopt;
		}
			
        // TODO: try catch ?

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


	std::unique_ptr<IPathResolver>
	CreateVfsResolver(std::span<const VfsMount> mounts)
	{
		MountTable table;
		for (const auto& m : mounts)
		{
			if (m.scheme.empty())
				continue;
			table.Mount(std::string(m.scheme), m.root);

			//std::cerr << m.scheme << m.root << "\n";
		}


		return std::make_unique<VfsResolver>(std::move(table));
	}
}// namespace ddknd::io