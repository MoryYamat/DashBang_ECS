#pragma once

#include <ddknd/asset/asset_tag.h>
#include <ddknd/asset/asset_id.h>

namespace app::component
{
    struct CharacterAnimationClipsComponent
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
        ::ddknd::asset::AssetID<::ddknd::asset::tag::AnimationClip> attack;
    };
}