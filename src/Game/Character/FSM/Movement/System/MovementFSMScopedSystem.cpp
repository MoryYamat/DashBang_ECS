#include "MovementFSMScopedSystem.hpp"

#include "Game/Character/FSM/Movement/Database/MovementFSMDatabase.hpp"

#include "Game/Character/FSM/Movement/StateModel/MovementStateComponent.hpp"

#include "Game/Character/Control/Movement/Component/Intent/MovementIntentComponent.h"



#include "Game/Character/FSM/Movement/MovementStateTags.hpp"


// TODO: FSMがアクティブかどうかの定義を
//		MovementFSMDefinitionに明示的に記述できるようにする
//		または，MovementStateComponent側で，FSM活性状態を示す bool を管理．
void Game::Character::FSM::Movement::System::MovementFSMScopedEffectSystem::Update(eNsECS::EntityMgr& ecs, float deltaTime)
{
	using namespace Game::Character::Intent;
	using namespace Game::Character::FSM::Movement;
	using namespace Game::Character::FSM::Movement::Database;

	const auto& db = ecs.getResource<MovementFSMDatabase>();

	const auto& def = db.Get("basic");// 切り替えは今後対応


	if (def.fsmScopedEffects.empty()) return;

	for (eNsECS::Entity e : ecs.view<
		MovementStateComponent,
		MovementIntentComponent
	>())
	{
		const auto& state = ecs.get<MovementStateComponent>(e);
		const auto& intent = ecs.get<MovementIntentComponent>(e);

		// IDLE/MOVINGのときだけFSMスコープエフェクトを適用
		if (state.current != StateTag::IDLE && state.current != StateTag::MOVING) continue;

		MovementFSMContext ctx;
		ctx.intentActive = intent.isActive;
		ctx.direction = intent.direction;

		//std::cout << "[MovementFSMScopedSystem] ctx.direction = ("
		//	<< ctx.direction.x << ", " << ctx.direction.y << ")\n";

		for (const auto& effect : def.fsmScopedEffects)
		{
			effect->update(ecs, e, ctx, deltaTime);
		}
	}
}