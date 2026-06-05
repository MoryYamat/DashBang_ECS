#pragma once
#include <string>
#include <string_view>
#include <vector>
#include <optional>

namespace ddknd::asset::parser
{
    namespace detail
	{
		std::string Trim(std::string_view s);

		std::string StripCommentAndTrim(std::string_view line);

		bool StartsWith(std::string_view s, std::string_view prefix);
	}

	struct ParsedMeshPos3
	{
		std::vector<float> xyz;	// x,y,z,...
	};

	// test_triangle.mesh 用 : layout=pos3 と vN= x y z を読む
	std::optional<ParsedMeshPos3> ParseMeshPos3Text(std::string_view text);
}