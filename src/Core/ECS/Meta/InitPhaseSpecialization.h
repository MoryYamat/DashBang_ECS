// 特定のComponent型に対して初期化タイミングを特殊化
// Specialize the initialization timing for a particular Component type.

#include "Core/ECS/Meta/InitPhase.h"

#include "Core/ECS/Component/TileMapComponent.h"

#include "Core/ECS/Component/FollowCameraComponent.h"


template<>// Specialize template テンプレート特殊化
constexpr InitPhase GetInitPhase<TileMapComponent>()
{
	return InitPhase::Deferred;
}

template<>
constexpr InitPhase GetInitPhase<FollowCameraComponent>()
{
	return InitPhase::Deferred;
}
