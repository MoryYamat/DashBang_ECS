#pragma once

#include <string>
#include <optional>
#include <filesystem>

namespace ddknd::io
{
    // Get all the contents of a file as a string 
	// @note: *****(be careful when using large files)*****
    std::optional<std::string>
    ReadAllText(const std::filesystem::path& path);
}