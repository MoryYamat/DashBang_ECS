#include "ddknd/asset/sub_asset_key.h"// required for `MakeAnimationClipKey`

namespace ddknd::asset
{
    // helper: 
    // Issue a logical path to the Animation data included as a SubAsset within the Model data
    std::string MakeAnimationClipKey(std::string_view modelPath, std::string_view animName)
    {
        std::string key;
        key.reserve(modelPath.size() + animName.size() + 8);
        key += modelPath;
        key += "#anim/";
        key += animName;
        return key;
    }
}