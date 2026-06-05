#pragma once

#include <string_view>
#include <string>
#include <vector>
#include <unordered_map>
#include <optional>

namespace ddknd::asset::parser
{
    struct KeyValueDoc
    {
        // (key , values) (e.g. vertex = res://shaders/src/pbr.vertex , v0 = -0.5 -0.5 0.0)
        std::unordered_map<std::string, std::vector<std::string>> values;
    };

    struct ParseError
    {
        std::string msg;
        int line = -1;
    };

    // TODO: Create IParser to provide diverse parsing capabilities.
    std::optional<KeyValueDoc>
        ParseKeyValueText(std::string_view text, ParseError* err);

    struct ShaderDesc
    {
        std::string vertex_path;
        std::string fragment_path;
        std::vector<std::string> include_paths;
    };

    std::optional<ShaderDesc>
        BuildShaderDesc(const KeyValueDoc& doc, ParseError* err);

    // trim space / return / carrage return 
	std::string_view Trim(std::string_view s);

	// find values
	const std::vector<std::string>* find_values(const KeyValueDoc& doc, std::string_view key);

}// namespace ddknd::graphics