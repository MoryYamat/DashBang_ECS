// Functions that provide auxiliary functions for ECS
// ECSの補助機能を提供する関数群
#pragma once

#include "Core/ECS/Entity.h"
#include "Core/ECS/EntityManager.h"

#include <cstdint>
#include <string>

namespace EntityUtils
{
	//uint32_t getEntityByName(ECS& ecs, const std::string& name);

	// For name resolution
	Entity getEntityByName(ECS& ecs, const std::string& name);

	// 親と関連子Entityを削除予約にマークする
	void MarkForPendingDestroyWithChildren(ECS& ecs, Entity parent, const std::vector<Entity>& children,
		int delayFrames = 0, bool fadeOut = false);

	// スキルインスタンスが子を持たない場合／オブジェクト単体を削除予約にマークする
	void MarkForPendingDestroy(ECS& ecs, Entity entity, int delayFrames = 0, bool fadeOut = false);
}