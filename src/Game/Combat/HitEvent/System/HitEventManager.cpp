#include "HitEventManager.hpp"

#include "Engine/Time/WorldClock.hpp"

#include "Game/Combat/HitEvent/Data/HitEvent.hpp"
#include "Game/Combat/HitEvent/Database/HitEventDatabase.hpp"

#include "Game/Character/FSM/CC/API/CCRequests.hpp"

#include "Game/Combat/Skill/MasterData/SkillDatabase.h"
#include "Game/Combat/Skill/MasterData/SkillEntry.hpp"
#include "Game/Combat/Skill/Def/SkillDef.hpp"

#include <iostream>

namespace Game::Combat::HitEvent::System
{
	using namespace Engine::Time;

	using namespace Game::Character::FSM::CC::API;

	using namespace Game::Combat::HitEvent::Data;
	using namespace Game::Combat::HitEvent::Database;
	using namespace Game::Combat::Skill::Database;
	using namespace Game::Combat::Skill::Data;
	using namespace Game::Combat::Skill::Def;

	void HitEventManager(EntityMgr& ecs)
	{
		// const auto& clock = worldClock(ecs);
		auto& skillDb = skillDatabase(ecs);

		auto& hitDb = hitEventDatabase(ecs);

		for (auto& ev : hitDb.items)
		{
			if (isCompleted(ev) || ev.CCExecutionFlag) continue;

			// nullptr は正常か？
			const SkillEntry* entry = skillDb.tryGet(ev.skillId);

			// std::cout << "[HitEventManager]: hit event was detected.\n";

			// 将来的にはテーブル化?
			if (!entry || !entry->def.cc.has_value())
			{
				markCompleted(ev);
				continue;
			}

			// Skill ID による CC種 解決
			const auto& cc = *entry->def.cc;
			// target に CCリクエストを送出するような関数呼び出し
			emitCCRequest(ecs, ev.target, cc.type, cc.priority);
			// ===

			std::cout << "[HitEvent] eventID = " << ev.eventId
				<< " skillID = " << ev.skillId
				<< " target= " << ev.target.id
				<< " -> emitCC(type= " << cc.type.name()
				<< ", priority= " << cc.priority << ")\n";

			markCCExecuted(ev);// 2重発火を防ぐ


			// FIXME: hiteventが何度も発火する現象あり．
			// → HitEventは固有のSkillHitboxとtarget対象について一つ(もしくはコントロール可能な回数)でなければならない
			// -> 同じskillhitboxについて同じEventIDでなければならない
			// 
			// ============一時的な措置===========
			// 1. hitbox側にhitboxmemoという衝突判定済み記録データ
			// 2. HitEvent初回発行時にチェック
			// 3. 早期スキップに活用
			// 
			markCompleted(ev);// hitevent の寿命終了

			// std::cout << "[HitEvent] skillId=" << ev.skillId
			// 	<< " done (completed)\n";
		}

		hitDb.erase_if([](const HitEvent& e) {return isCompleted(e);});
	}


}