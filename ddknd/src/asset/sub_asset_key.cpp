#include "ddknd/asset/sub_asset_key.h"// required for `MakeAnimationClipKey`

namespace ddknd::asset
{
    // helper: 
    // Issue a logical path to the Animation data included as a SubAsset within the Model data
    std::string MakeAnimationClipKey(std::string_view modelPath, std::string_view animName)
    {
        constexpr std::string_view sep = "#anim/";
        std::string key;
        key.reserve(modelPath.size() + animName.size() + sep.size());
        key += modelPath;
        key += sep;
        key += animName;
        return key;
    }

    
    // Issue a logical path to the Texture data included as a SubAsset within the Model data
    std::string MakeTextureKey(std::string_view modelPath, std::size_t textureIndex)
    {
        constexpr std::string_view sep = "#texture/";
        const auto indexText = std::to_string(textureIndex);
        
        std::string key;
        key.reserve(modelPath.size() + indexText.size() + sep.size());
        key += modelPath;
        key += sep;
        key += indexText;
        return key;
    }
}