#include "Engine/FSM/Public/Core/Types.hpp"

#include <cstdint>
#include <cstddef>

namespace Engine::FSM::Core
{
	[[nodiscard]] Decision TransitionSelector
	(
		StateID from,
		const TransitionView_CSR& tv,
		const CondTable& ct,
		const EnvSnapshot& env,
		const EvalCtx& ctx
	)
	{
		Decision d{ from, {}, false, Reason::None };

		const auto outs = tv.outgoing(from);
		if (outs.empty()) return d;

		for (const Transition& tr : outs)
		{
			if (!ct.eval(tr.cond, env, ctx)) continue;
			d.to = tr.to;
			d.cond = tr.cond;
			d.changed = (d.to.id != from.id);
			d.reason = Reason::FirstTrue;
			
			if (tr.prio == kMaxPrio) return d;
			return d;// 先頭が最良(降順ソート済みのため)

		}

		return d;
	}
}