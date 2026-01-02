#include "Game/Combat/Skill/Public/RuntimeAPI.hpp"

#include "Game/Character/FSM/Public/MovementAxisComponent.hpp"

// hitbox
#include "Game/Combat/HitBox/Internal/HitBoxTypes.hpp"
#include "Game/Combat/HitBox/Public/HitBoxFwd.hpp"

// anim
#include "Game/Combat/Animation/Internal/AnimTypes.hpp"
#include "Game/Combat/Animation/Public/AnimationFwd.hpp"

#include "Game/Combat/Skill/Internal/Binding/BindingTypes.hpp"

#include "Game/Combat/Skill/Internal/Runtime/RuntimeTypes.hpp"

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
			case LogicCommandKind::ModifyMoveSpeed:
				HandleModifyMoveSpeed(cmd);
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

		// std::cout << "here1\n";

		buffer.reqs.push_back(r);
	}

	void SkillCommandExecSystem::HandleModifyMoveSpeed(const SkillLogicCommand& cmd)
	{
		using namespace Game::Character::FSM::Movement;

		auto* mods = ctx.ww.TryGet<MovementModifierComponent>(cmd.owner);
		if (!mods)
		{
			return;
		}

		MovementModifierEntry e{};
		e.mul = cmd.value;
		e.add = 0.f;
		e.remaining = cmd.lifetime;

		mods->entries.push_back(e);
	}

	// サブシステム用リクエストに変換
	void SkillCommandExecSystem::HandlePlayerAnim(const SkillLogicCommand& cmd)
	{
		using namespace ::Game::Combat::Animation;

		auto& buf = ctx.ww.GetResource<PlayAnimRequestBuffer>();
		auto& binding = ctx.ww.GetResource<Binding::SkillBindingData>();


		AnimID id = binding.resolveAnim(cmd.skill, cmd.state);
		if (!id.valid())
		{
			//TODO: エラー種別を判定する方法を考えるべき → skill / state どちらに 不一致があるのか
			
			Engine::Log::Write(Engine::Log::Level::Error, "SkillCommandExecSystem",
				"AnimMap failed to resolve AnimID");
			return;
		}
		// 
		// AnimPlayRequest r{};
		// r.owner = cmd.owner;
		// r.skill = cmd.skill;
		// r.state = cmd.state;
		// // 将来：キャラの種類やスキルから AnimClip を引く
		// 
		// animReqBuf.reqs.push_back(r);

		PlayAnimRequest r{};
		r.anim = id;
		r.owner = cmd.owner;
		r.skill = cmd.skill;
		r.state = cmd.state;

		buf.reqs.push_back(std::move(r));

		// std::cout << "here2222\n";
	}
}