#include "MovementAnimationResolverSystem.hpp"


#include "Game/Character/Animation/Profile/AnimationProfileComponent.hpp"

#include "Game/Character/Animation/Profile/Movement/MovementAnimationProfile.hpp"


#include "Game/Character/Animation/Query/AnimationQueryComponent.hpp"
#include "Game/Character/Animation/Resolve/Movement/MovementAnimDecisionComponent.hpp"

#include "Engine/ECS/Ops/CoreOps.hpp"

#include "Engine/Math/Logic2D/LogicMathUtils.h"

#include <glm/gtc/constants.hpp>

#include <cassert>


namespace Game::Character::Animation::Resolve::Movement
{
	namespace ECS = Engine::ECS;
	namespace Ops = Engine::ECS::Ops;

	namespace Query = Game::Character::Animation::Query;
	namespace AnimResolve = Game::Character::Animation::Resolve::Movement;
	namespace Prof = Game::Character::Animation::Profile;

	namespace Math2D = Engine::Math::Logic2D;

	void MovementAnimationResolverSystem::Update(ECS::EntityMgr& ecs)
	{
		auto& db = Ops::GetOrCreateRes<Prof::MovementAnimationProfileDatabase>(ecs);

		for (auto e : ecs.view<
			Query::AnimationQueryComponent,
			AnimResolve::MovementAnimDecisionComponent,
			Prof::AnimationProfileComponent
		>())
		{
			const auto& q = Ops::Get<Query::AnimationQueryComponent>(ecs, e);
			const auto& prof = Ops::Get<Prof::AnimationProfileComponent>(ecs, e);
			auto& dec = Ops::Get<AnimResolve::MovementAnimDecisionComponent>(ecs, e);



			if (!q.valid)
			{
				dec.valid = false;
				continue;
			}

			dec.type = q.isMoving ? calcRelativeMovementDir(q.facingDirWorld, q.moveDirWorld) : AnimResolve::MoveAnimType::Idle;


			const auto* mp = db.FindMovement(prof.profileId);
			assert(mp);
		



			// TODO: switchは著しく柔軟性を損なうため、ほかの方法によって解決できるようにしたい
			switch (dec.type)
			{
			case AnimResolve::MoveAnimType::Idle: dec.clipKey = mp->idle.empty() ? "idle_default" : mp->idle; break;
			case AnimResolve::MoveAnimType::RunFwd: dec.clipKey = mp->runFwd.empty() ? "run_fwd_default" : mp->runFwd; break;
			case AnimResolve::MoveAnimType::RunBack: dec.clipKey = mp->runBack.empty() ? "run_back_default" : mp->runBack; break;
			case AnimResolve::MoveAnimType::RunRight: dec.clipKey = mp->runRight.empty() ? "run_right_default" : mp->runRight; break;
			case AnimResolve::MoveAnimType::RunLeft: dec.clipKey = mp->runLeft.empty() ? "run_left_default" : mp->runLeft; break;
			}

			dec.loop = true;
			dec.playRate = 1.0f;
			dec.valid = true;
		}
	}

	Game::Character::Animation::Resolve::Movement::MoveAnimType 
		MovementAnimationResolverSystem::calcRelativeMovementDir(const glm::vec2 facingDir, const glm::vec2 movingDir)
	{
		constexpr float kEpsLen2 = 1e-8f;
		constexpr float k45 = glm::pi<float>() / 4.0f; // 45
		constexpr float k135 = 3.0f * glm::pi<float>() / 4.0f;// 135

		glm::vec2 f = (Math2D::Vector::SquaredLength(facingDir)) > 0.f ? glm::normalize(facingDir) : glm::vec2(0, 1);
		glm::vec2 m = (Math2D::Vector::SquaredLength(movingDir)) > 0.f ? glm::normalize(movingDir) : f;

		// +Z 軸 基準 [0, 2pi)
		const float yawF = Math2D::CalcYawFromDirection(f);
		const float yawM = Math2D::CalcYawFromDirection(m);

		const float yaw = Math2D::ToSignedPi(yawM - yawF); // [-pi, pi]
		const float ay = std::abs(yaw);

		if (ay < k45) return AnimResolve::MoveAnimType::RunFwd;
		else if (ay > k135) return AnimResolve::MoveAnimType::RunBack;
		else if (yaw < 0.0f) return AnimResolve::MoveAnimType::RunRight;
		else return AnimResolve::MoveAnimType::RunLeft;

		// fwd

		// back

		// righ 

		// left
	}
}