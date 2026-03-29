#include "internal/asset/shader_descriptor_parser.h"

#include <spdlog/spdlog.h>

namespace ddknd::asset::parser
{
    /// <summary>
    /// in (text = ファイル内のすべての文字列, err = エラー用)
    ///
    /// 概要
    /// - 1行ずつ取り出す(find('\n'))
    /// - '='を区切に(key,value)に分ける(find('='))
    /// - 結果を追加して、次の行へ
    /// </summary>
    // TODO: Create IParser to provide diverse parsing capabilities.
    // parse syntax: key = value
    std::optional<KeyValueDoc> ParseKeyValueText(std::string_view text, ParseError* err)
    {
        KeyValueDoc res;

        int line_no = 1;
        while (!text.empty())
        {
            auto pos = text.find('\n');
            std::string_view line = (pos == std::string_view::npos) ? text : text.substr(0, pos);

            text = (pos == std::string_view::npos) ? std::string_view{} : text.substr(pos + 1);

            // get comment symbol(#) pos
            if (auto c = line.find('#'); c != std::string_view::npos)
                line = line.substr(0, c);

            line = Trim(line);
            if (line.empty())
            {
                ++line_no;
                continue;
            }

            auto eq = line.find('=');
            if (eq == std::string_view::npos)
            {
                if (err)
                {
                    err->line = line_no;
                    err->msg = "expected '='";
                }
                return std::nullopt;
            }

            auto key = Trim(line.substr(0, eq));
            auto value = Trim(line.substr(eq + 1));

            if (key.empty() || value.empty())
            {
                if (err)
                {
                    err->line = line_no;
                    err->msg = "empty key or value";
                }
                return std::nullopt;
            }

            res.values[std::string(key)].push_back(std::string(value));
            ++line_no;
        }

        return res;
    }

    /// <summary>
    /// 必須: "vertex"/"fragment" オプション: "include"
    ///
    /// 必須key の [0] を src を持つ vpath として取り出す
    /// include は すべて を src を持つ vapth として取り出す
    /// </summary>
    std::optional<ShaderDesc> BuildShaderDesc(const KeyValueDoc& doc, ParseError* err)
    {
        ShaderDesc out;
        {
            const auto* v = find_values(doc, "vertex");
            if (!v || v->empty() || (*v)[0].empty())
            {
                if (err)
                    err->msg = "missing required key: vertex";
                return std::nullopt;
            }
            out.vertex_path = (*v)[0];
            if (v->size() > 1)
            {
                if (err)
                    err->msg = "key 'vertex' must appear only once";
                return std::nullopt;
            }
        }

        {
            const auto* f = find_values(doc, "fragment");
            if (!f || f->empty() || (*f)[0].empty())
            {
                if (err)
                    err->msg = "missing required key: fragment";
                return std::nullopt;
            }
            out.fragment_path = (*f)[0];
            if (f->size() > 1)
            {
                if (err)
                    err->msg = "key 'fragment' must apper only once";
                return std::nullopt;
            }
        }

        // include (optional multi)
        if (const auto* v = find_values(doc, "include"))
        {
            out.include_paths.reserve(v->size());
            for (const auto& s : *v)
            {
                if (!s.empty())
                    out.include_paths.push_back(s);
            }
        }

        return out;
    }

    // Trim spaces/ returns/ carriage_returns from a line.
    std::string_view Trim(std::string_view s)
    {
        const auto is_space = [](char c) { return c == ' ' || c == '\t' || c == '\r'; };

        while (!s.empty() && is_space(s.front()))
            s.remove_prefix(1);
        while (!s.empty() && is_space(s.back()))
            s.remove_suffix(1);
        return s;
    }

    // find values
    const std::vector<std::string>* find_values(const KeyValueDoc& doc, std::string_view key)
    {
        auto it = doc.values.find(static_cast<std::string>(key));
        if (it == doc.values.end())
        {
            spdlog::warn("shader descriptor error[find_values]: The value for key: {} does not exist. \n", key);
            return nullptr;
        }
        return &it->second;
    }
} // namespace ddknd::asset