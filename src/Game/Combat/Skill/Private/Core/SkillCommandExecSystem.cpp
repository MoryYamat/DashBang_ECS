#include "Game/Combat/Skill/Public/Core/SkillEffectTypes.hpp"


#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"

#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <iostream>

namespace Game::Combat::Skill
{
	void SkillCommandExecSystem::Update(const float dt)
	{
		auto& buf = ctx.ww.GetResource<SkillLogicCommandBuffer>();

		// if (buf.cmds.empty())
		// return;

		for (const auto& cmd : buf.cmds)
		{
			switch (cmd.kind)
			{
			case LogicCommandKind::SpawnHitBox:
				HandleSpawnHitBox(cmd);
				break;
			case LogicCommandKind::PlayAnim:
				HandlePlayerAnim(cmd);
				break;
			}
		}
	}

	using namespace Game::Combat::HitBox;

	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandleSpawnHitBox(const SkillLogicCommand& cmd)
	{
		// auto& hitboxReqBuf = ctx.ww.GetResource<HitBoxSpawnRequestBuffer>();
		// auto& hitboxMap = ctx.ww.GetResource<SkillHitBoxMapping>();

		// HitBoxID hb = hitboxMap.Resolve(cmd.skill);
		//if (!hb.valid())
		//{
		//	Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem", "HitboxMap failed to resolve HitBoxID");

		//	return;
		//}


		// HitBoxSpawnRequest r{};
		// r.owner = cmd.owner;
		// r.skill = cmd.skill;
		// r.state = cmd.state;
		// r.triggerTime = cmd.effectTime;
		// 将来：キャラごとの設定やスキル種別で HitboxID を解決して入れる

		std::cout << "here1\n";

		// hitboxReqBuf.reqs.push_back(r);
	}


	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandlePlayerAnim(const SkillLogicCommand& cmd)
	{
		// auto& animReqBuf = ctx.ww.GetResource<AnimPlayRequestBuffer>();
		// 
		// AnimPlayRequest r{};
		// r.owner = cmd.owner;
		// r.skill = cmd.skill;
		// r.state = cmd.state;
		// // 将来：キャラの種類やスキルから AnimClip を引く
		// 
		// animReqBuf.reqs.push_back(r);
		std::cout << "here2\n";
	}
}