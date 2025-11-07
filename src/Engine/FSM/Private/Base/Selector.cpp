#include "Engine/FSM/Public/Base/Types.hpp"




namespace Engine::FSM::Base
{
	Decision DecideNext_BySingleSlot
	(
		const CanonicalFSM& f,
		const AxisRuntime& ax,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::uint32_t slotLocal,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	)
	{
		Decision d{ fromLocal, fromLocal, false };

		const std::uint32_t N = f.numStates;
		const std::uint32_t P = f.numProfiles;
		const std::uint32_t S = f.numSlots;

		if (fromLocal >= N || profileLocal >= P || slotLocal >= S) return d;

		// profileSlot → CondID 
		const CondID cid = f.condOf[profileLocal * S + slotLocal];
		if (!cid.valid()) return d;

		// 条件評価
		if (!ax.conds.eval(cid, env, ctx)) return d;

		// CSRセル → 最初のエッジ採用 (prio降順)
		const std::uint32_t cell = fromLocal * S + slotLocal;
		const std::uint32_t a = f.ofs[cell], b = f.ofs[cell + 1];
		if (a == b) return d;

		d.to = f.edges[a].toIdx;
		d.changed = (d.to != d.from);
		return d;
	}

	Decision DecideNext_Slots
	(
		const CanonicalFSM& f,
		const AxisRuntime& ax,
		std::uint32_t fromLocal,
		std::uint32_t profileLocal,
		std::span<const std::uint32_t> slots,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	)
	{
		for (std::uint32_t s : slots)
		{
			auto d = DecideNext_BySingleSlot(f, ax, fromLocal, profileLocal, s, env, ctx);
			if (d.changed) return d;
		}
		return Decision{ fromLocal, fromLocal, false };
	}
}