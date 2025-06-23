#include "AttackLifetimeSystem.hpp"

#include "Game/Combat/Skill/Component/Attack2DAreaComponent.h"

#include "Engine/ECS/EntityUtils/EntityUtils.h"

#include "Common/GameNamespaceDecl.h"

void Game::Combat::Skill::System::AttackLifetimeSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	for (eNsECS::Entity e : ecs.view<gNsSkillComp::Attack2DAreaComponent>())
	{
		auto& area = ecs.get<gNsSkillComp::Attack2DAreaComponent>(e);
		area.lifetime -= deltaTime; // ライフタイムを減少
		if (area.lifetime <= 0.0f)
		{
			// ライフタイムが終了したら、エンティティを削除
			std::cout << "[AttackLifetimeSystem] Attack area with owner " << area.owner.id << " has expired and will be destroyed.\n";
			eNsECS::EntityUtils::MarkForPendingDestroy(ecs, e);
		}
		else
		{
			// ライフタイムが残っている場合は、必要に応じて更新処理を行う
			// Update logic if needed while the lifetime is still active
		}
	}
}