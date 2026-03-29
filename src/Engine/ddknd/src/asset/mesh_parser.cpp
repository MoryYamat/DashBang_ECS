#include "internal/asset/mesh_text_parser.h"

#include <array>
#include <istream>
#include <sstream>
#include <unordered_map>
#include <cctype>

namespace ddknd::asset::parser
{
    namespace detail
    {
        // trim space
        std::string Trim(std::string_view s)
        {
            std::size_t b = 0;
            while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b])))
                b++;
            std::size_t e = s.size();
            while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1])))
                e--;

            return std::string(s.substr(b, e - b));
        }

        // 
        std::string StripCommentAndTrim(std::string_view line)
        {
            const auto pos = line.find('#');
            if (pos != std::string_view::npos)
                line = line.substr(0, pos);
            return Trim(line);
        }

        bool StartsWith(std::string_view s, std::string_view prefix)
        {
            return s.size() >= prefix.size() && s.substr(0, prefix.size()) == prefix;
        }
    } // namespace detail

    // test_triangle.mesh 用 : layout=pos3 と vN= x y z を読む
    std::optional<ParsedMeshPos3> ParseMeshPos3Text(std::string_view text)
    {
        bool layoutPos3 = false;

        // v0, v1, ...
        std::unordered_map<int, std::array<float, 3>> verts;

        std::string owned(text);
        std::istringstream iss(owned);
        std::string line;
        while (std::getline(iss, line))
        {
            auto s = detail::StripCommentAndTrim(line);
            if (s.empty())
                continue;

            // key = value
            const auto eq = s.find('=');
            if (eq == std::string::npos)
                continue;

            const auto key = detail::Trim(std::string_view(s).substr(0, eq));
            const auto val = detail::Trim(std::string_view(s).substr(eq + 1));

            if (key == "layout")
            {
                layoutPos3 = (val == "pos3");
                continue;
            }

            // vN
            if (!key.empty() && key[0] == 'v')
            {
                // v0 / v1 / v2 ...
                int idx = -1;
                try
                {
                    idx = std::stoi(key.substr(1));
                }
                catch (...)
                {
                    return std::nullopt;
                }

                std::istringstream vs(val);
                float x = 0, y = 0, z = 0;
                if (!(vs >> x >> y >> z))
                    return std::nullopt;

                verts[idx] = {x, y, z};
            }
        }

        if (!layoutPos3)
            return std::nullopt;

        // v0..vN
        ParsedMeshPos3 out;
        if (verts.empty())
            return std::nullopt;

        int maxIdx = -1;
        for (const auto& [k, _] : verts)
        {
            if (k > maxIdx)
                maxIdx = k;
        }

        for (int i = 0; i <= maxIdx; ++i)
        {
            auto it = verts.find(i);
            if (it == verts.end())
                return std::nullopt;

            out.xyz.push_back(it->second[0]);
            out.xyz.push_back(it->second[1]);
            out.xyz.push_back(it->second[2]);
        }

        return out;
    }
} // namespace ddknd::asset::parser