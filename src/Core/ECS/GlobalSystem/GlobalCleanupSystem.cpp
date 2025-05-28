#include "GlobalCleanupSystem.h"

#include "Core/ECS/Component/Tags/PendingDestroyComponent.h"

void GrobalSystem::RunCleanup(class ECS& ecs)
{
	for (Entity e : ecs.view<PendingDestroyComponent>())
	{
		auto& destroy = ecs.get<PendingDestroyComponent>(e);

		// 削除遅延フレーム消費
		if (destroy.delayFrames > 0)
		{
			destroy.delayFrames--;
			continue;
		}

		if (destroy.fadeOutEffect)
		{
			// フェードアウト演出処理...
		}

		ecs.destroyEntity(e);
	}
}