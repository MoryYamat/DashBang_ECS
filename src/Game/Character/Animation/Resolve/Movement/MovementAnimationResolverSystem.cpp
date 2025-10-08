#include "MovementAnimationResolverSystem.hpp"


#include "Game/Character/Animation/Profile/AnimationProfileComponent.hpp"

#include "Game/Character/Animation/Profile/Movement/MovementAnimationProfile.hpp"


#include "Game/Character/Animation/Query/AnimationQueryComponent.hpp"
#include "Game/Character/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"

#include "Game/ECS/Tags/CharacterAttribTags.h"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <glm/gtc/constants.hpp>

#include <cassert>

// TODO: アニメーション切り替え境界時に 「ガクッ」という感じがある
// アニメーション間のブレンドや「ヒステリシス」などによって対策が必要
// 現在のところは考えない(2025/10/08)
namespace Game::Character::Animation::Resolve::Movement
{
	namespace ECS = Engine::ECS;
	namespace Ops = Engine::ECS::Ops;

	namespace Query = Game::Character::Animation::Query;
	namespace ARes = Game::Character::Animation::Resolve::Movement;
	namespace Prof = Game::Character::Animation::Profile;

	namespace Math2D = Engine::Math::Logic2D;

	void MovementAnimationResolverSystem::Update(ECS::EntityMgr& ecs)
	{
		using Type = ARes::MoveAnimType;
		auto& db = Ops::GetOrCreateRes<Prof::MovementAnimationProfileDatabase>(ecs);

		for (auto e : ecs.view<
			Query::AnimationQueryComponent,
			ARes::MovementAnimDecisionComponent,
			Prof::AnimationProfileComponent
		>())
		{
			const auto& q = Ops::Get<Query::AnimationQueryComponent>(ecs, e);
			const auto& prof = Ops::Get<Prof::AnimationProfileComponent>(ecs, e);
			auto& dec = Ops::Get<ARes::MovementAnimDecisionComponent>(ecs, e);



			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}
			const bool isPlayer = Ops::Has<Game::ECS::Tags::PlayerCharacterTag>(ecs, e);
			const Type want = q.isMoving ? calcRelativeMovementDir(q.facingDirWorld, q.moveDirWorld, dec.type) : ARes::MoveAnimType::Idle;


			const auto* mp = db.FindMovement(prof.profileId);
			assert(mp);
		
			if(!isPlayer)
			{
				dec.type = quantizeTo4Dir(want);
			}
			else
			{
				// ② デバウンス処理
				if (want == dec.type)
				{
					// 同じ方向ならリセット
					dec.pendingFrames = 0;
					dec.pending = want;
				}
				else
				{
					if (dec.pending != want)
					{
						// 新しい候補方向が出たのでリセット
						dec.pending = want;
						dec.pendingFrames = 0;
					}
					else
					{
						// 同じ候補方向が続いている
						if (++dec.pendingFrames >= 3) // ←ここが閾値（3フレーム連続）
						{
							dec.type = dec.pending;
							dec.pendingFrames = 0;
						}
					}
				}
			}



			// TODO: switchは著しく柔軟性を損なうため、ほかの方法によって解決できるようにしたい
			switch (dec.type)
			{
			case Type::Idle: dec.clipKey = mp->idle.empty() ? "idle_default" : mp->idle; break;
			case Type::RunFwd: dec.clipKey = mp->runFwd.empty() ? "run_fwd_default" : mp->runFwd; break;
			case Type::RunBack: dec.clipKey = mp->runBack.empty() ? "run_back_default" : mp->runBack; break;
			case Type::RunRight: dec.clipKey = mp->runRight.empty() ? "run_right_default" : mp->runRight; break;
			case Type::RunLeft: dec.clipKey = mp->runLeft.empty() ? "run_left_default" : mp->runLeft; break;
			case Type::RunFwdLeft:   dec.clipKey = mp->runFwdLeft.empty() ? "run_fwd_left_default" : mp->runFwdLeft;  break;
			case Type::RunFwdRight:  dec.clipKey = mp->runFwdRight.empty() ? "run_fwd_right_default" : mp->runFwdRight; break;
			case Type::RunBackLeft:  dec.clipKey = mp->runBackLeft.empty() ? "run_back_left_default" : mp->runBackLeft; break;
			case Type::RunBackRight: dec.clipKey = mp->runBackRight.empty() ? "run_back_right_default" : mp->runBackRight;break;

			}

			dec.loop = true;
			dec.playRate = 1.0f;
			dec.valid = true;
		}
	}

	Game::Character::Animation::Resolve::Movement::MoveAnimType 
		MovementAnimationResolverSystem::calcRelativeMovementDir(const glm::vec2 facingDir, const glm::vec2 movingDir,
			const Game::Character::Animation::Resolve::Movement::MoveAnimType prev)
	{
		using Type = ARes::MoveAnimType;

		constexpr float k45 = glm::radians(45.0f);
		constexpr float k135 = glm::radians(135.0f);
		constexpr float kHys = glm::radians(5.0f);

		const float k45on = k45 + kHys;
		const float k45off = k45 - kHys;
		const float k135on = k135 + kHys;
		const float k135off = k135 - kHys;

		glm::vec2 f = (Math2D::Vector::SquaredLength(facingDir)) > 0.f ? glm::normalize(facingDir) : glm::vec2(0, 1);
		glm::vec2 m = (Math2D::Vector::SquaredLength(movingDir)) > 0.f ? glm::normalize(movingDir) : f;

		// +Z 軸 基準 [0, 2pi)
		const float yawF = Math2D::CalcYawFromDirection(f);
		const float yawM = Math2D::CalcYawFromDirection(m);

		// const float yaw = Math2D::ToSignedPi(yawM - yawF); // [-pi, pi]
		const float yaw = Math2D::ToSignedPi(yawF - yawM); // [-pi, pi]
		const float ay = std::abs(yaw);
		

		//  TODO: switchはできるだけ避けたい->今後ほかの良い方法を考える
		// 最適化必要
		//switch(prev)
		//{
		//case Type::RunFwd:
		//	if (ay <= k45off) return Type::RunFwd;// 前を維持
		//	break;
		//case Type::RunBack:
		//	if (ay >= k135off) return Type::RunBack;// 後ろを維持
		//	break;
		//case Type::RunLeft:
		//	if (ay <= k45off) return Type::RunFwd;
		//	if (ay >= k135on) return Type::RunBack;
		//	if (yaw > 0.0f) return Type::RunLeft;

		//	return Type::RunRight;

		//case Type::RunRight:
		//	if (ay <= k45off) return Type::RunFwd;
		//	if (ay >= k135on) return Type::RunBack;
		//	if (yaw < 0.0f) return Type::RunRight;

		//	return Type::RunLeft;

		//default:
		//	break;
		//}

		// yaw を degree に
		float deg = glm::degrees(yaw);
		if (deg < 0) deg += 360.f;

		

		// 8方向分割（22.5°ごと）
		if(deg < 22.5f || deg >= 337.5f) return Type::RunFwd;
		if (deg < 67.5f)  return Type::RunFwdRight;
		if (deg < 112.5f) return Type::RunRight;
		if (deg < 157.5f) return Type::RunBackRight;
		if (deg < 202.5f) return Type::RunBack;
		if (deg < 247.5f) return Type::RunBackLeft;
		if (deg < 292.5f) return Type::RunLeft;
		return Type::RunFwdLeft;



		// 通常判定
		if (ay < k45on) return Type::RunFwd;
		else if (ay > k135on) return Type::RunBack;
		else if (yaw < 0.0f) return Type::RunRight;
		else return Type::RunLeft;

		// fwd

		// back

		// righ 

		// left
	}

	// TODO: 最適化
	// TODO: 最適化
	// TODO: 最適化
	Game::Character::Animation::Resolve::Movement::MoveAnimType
		MovementAnimationResolverSystem::quantizeTo4Dir
		(const Game::Character::Animation::Resolve::Movement::MoveAnimType type)
	{
		using Type = ARes::MoveAnimType;

		switch (type)
		{
		case Type::RunFwdLeft:
		case Type::RunFwdRight:
			return Type::RunFwd;

		case Type::RunBackLeft:
		case Type::RunBackRight:
			return Type::RunBack;

		default:
			return type; // それ以外はそのまま（Idle, RunLeft, RunRight）
		}
	}
}