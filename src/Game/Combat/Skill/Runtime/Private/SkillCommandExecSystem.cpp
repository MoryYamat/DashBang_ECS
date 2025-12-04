#include "Game/Combat/Skill/Runtime/Public/SkillRuntimeAPI.hpp"

#include "Game/Combat/HitBox/Public/HitBoxTypes.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"

#include "Game/Combat/Skill/Binding/Public/Types.hpp"


#include "Engine/WorldSystem/Private/AllWorldSystem.hpp"
#include "Engine/Log/Public/LogApi.hpp"

#include <iostream>

namespace Game::Combat::Skill::Runtime
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
				HandlePlayerAnim(cmd);// 未作成
				break;
			}
		}
	}

	using namespace Game::Combat::HitBox;

	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandleSpawnHitBox(const SkillLogicCommand& cmd)
	{
		auto& buffer = ctx.ww.GetResource<HitBoxSpawnRequestBuffer>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();

		HitBoxID hb = binding.resolveHitBox(cmd.skill, cmd.state);
		if (!hb.valid())
		{
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem", 
				"HitboxMap failed to resolve HitBoxID");
			return;
		}


		HitBoxSpawnRequest r{};
		r.hitbox = hb;
		r.owner = cmd.owner;
		r.skill = cmd.skill;
		r.state = cmd.state;
		r.triggerTime = cmd.effectTime;
		r.lifetime = cmd.lifetime;
		// 将来：キャラごとの設定やスキル種別で HitboxID を解決して入れる

		std::cout << "here1\n";

		buffer.reqs.push_back(r);
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