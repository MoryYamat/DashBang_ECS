// �����Component�^�ɑ΂��ď������^�C�~���O����ꉻ
// Specialize the initialization timing for a particular Component type.

#include "Core/ECS/Meta/InitPhase.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Core/ECS/Component/FollowCameraComponent.h"


template<>// Specialize template �e���v���[�g���ꉻ
constexpr InitPhase GetInitPhase<TileMapComponent>()
{
	return InitPhase::Deferred;
}

template<>
constexpr InitPhase GetInitPhase<FollowCameraComponent>()
{
	return InitPhase::Deferred;
}
