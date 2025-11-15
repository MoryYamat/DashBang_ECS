#pragma once

#include <cstdint>
#include <unordered_map>


namespace Engine::FSM::Core
{
	inline constexpr std::uint32_t kInvalidID = UINT32_MAX;

	struct PerStateProfilePlan;
	struct PrecomputedEvalPlans;
	struct NextEvalSet;
	

	class FSMRegistry;
	struct CanonicalAxis;
	struct EnvAssemblerPlan;
	struct Decision;


	struct AxisInstance;

}