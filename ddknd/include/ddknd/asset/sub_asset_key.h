#pragma once

#include <cstddef>
#include <string>
#include <string_view>


namespace ddknd::asset
{
    std::string MakeAnimationClipKey(std::string_view modelPath, std::string_view animName);

    std::string MakeTextureKey(std::string_view modelPath, std::size_t textureIndex);
} // namespace ddknd::asset