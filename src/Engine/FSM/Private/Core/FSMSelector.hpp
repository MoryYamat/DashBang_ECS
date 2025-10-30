//#pragma once
//
//#include "Engine/FSM/Public/Core/Types.hpp"
//
//namespace Engine::FSM::Core
//{
//	Decision TransitionSelector
//	(
//		StateID from,
//		const TransitionView_CSR& tv,
//		const CondTable& conditions,
//		const EnvSnapshot& env,
//		const EvalCtx& ctx
//	)
//	{
//		Decision d{ from };
//		const auto outs = tv.outgoing(from);
//
//		std::uint8_t bestPrio = 0;
//		bool found = false;
//
//		for (size_t i = 0; i < outs.size(); ++i)
//		{
//			const auto& tr = outs[i];
//			if (!conditions.eval(tr.cond, env, ctx))continue;
//
//			if (!found)
//			{
//				d.to = tr.to;
//				d.cond = tr.cond;
//				d.changed = (d.to.id != from.id);
//				d.reason = Reason::FirstTrue;// 
//				bestPrio = tr.prio;
//				found = true;
//
//				if (bestPrio == kMaxPrio) return d;// 最高優先度なら即確定
//				continue;
//			}
//
//			if (tr.prio > bestPrio)
//			{
//				// より高優先度が出たら差し替え
//				d.to = tr.to;
//				d.cond = tr.cond;
//				d.changed = (d.to.id != from.id);
//				d.reason = Reason::HigherPrio;          // higher_prio
//				bestPrio = tr.prio;
//				if (bestPrio == kMaxPrio) return d; // 任意の早期終了
//			}
//		}
//
//		return d;
//	}
//}
