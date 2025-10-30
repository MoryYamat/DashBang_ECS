#pragma once

#include <cstdint>
#include <unordered_map>

namespace Engine::FSM::Core
{
	struct AuthoringFSM;
	struct AuthoringTransition;
	struct CanonicalAxis;

	struct AxisID;
	struct StateID;
	struct CondID;
	struct Transition;
	struct EntityKey;
	struct EvalCtx;
	struct EnvSnapshot;
	struct CondTable;
	struct TransitionView;
	enum class Reason : std::uint8_t;
	struct [[nodiscard]] Decision;

	class FSMRegistry;

	struct CondTableStaging;

	struct FSMCatalog;
	struct FSMCondTables;
}