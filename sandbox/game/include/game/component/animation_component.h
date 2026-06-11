#pragma once

#include <ddknd/asset/asset_tag.h>
#include <ddknd/asset/asset_manager.h>// required for AssetID
// #include <ddknd/graphics/gfx_type.h>

namespace app::component
{
    struct PlayerAnimationClipsComponent
    {
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> idle;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runForward;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runBackward;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRight;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeft;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRightFowardDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeftFowardDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runRightBackDiagonal;
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> runLeftBackDiagonal;
    };
}