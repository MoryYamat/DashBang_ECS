// Functions that provide auxiliary functions for ECS
// ECSの補助機能を提供する関数群
#pragma once

#include "Engine/ECS/Entity.h"
#include "Engine/ECS/EntityManager.h"

#include <cstdint>
#include <string>

namespace Engine::ECS
{
	namespace EntityUtils
	{
		// For name resolution
		eNsECS::Entity getEntityByName(eNsECS::EntityMgr& ecs, const std::string& name);

		// 親と関連子Entityを削除予約にマークする
		void MarkForPendingDestroyWithChildren(eNsECS::EntityMgr& ecs, eNsECS::Entity parent, const std::vector<eNsECS::Entity>& children,
			int delayFrames = 0, bool fadeOut = false);

		// スキルインスタンスが子を持たない場合／オブジェクト単体を削除予約にマークする
		void MarkForPendingDestroy(eNsECS::EntityMgr& ecs, eNsECS::Entity entity, int delayFrames = 0, bool fadeOut = false);
	}
}

