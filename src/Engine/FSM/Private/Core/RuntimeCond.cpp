#include "Engine/FSM/Public/FSMApi.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include <unordered_map>
#include <vector>
#include <cstdint>
#include <string>
#include <string_view>
#include <span>
#include <iostream>

namespace Engine::FSM::Core
{
	//bool SetupAxisRuntimeConds
	//(
	//	AxisRuntime& rt,// 評価器
	//	const CanonicalAxis& ax,
	//	std::span<const NamedCondBinding> bindings
	//)
	//{
	//	rt.canon = &ax;

	//	rt.conds.init(ax.condOrder.size());

	//	// name → index
	//	std::unordered_map<std::string_view, std::uint32_t> name2idx;
	//	name2idx.reserve(ax.condOrder.size());
	//	for (std::uint32_t i = 0; i < ax.condOrder.size(); ++i)
	//	{
	//		name2idx.emplace(std::string_view{ax.condOrder[i]}, i);
	//	}

	//	// bind
	//	std::vector<bool> hit(ax.condOrder.size(), false);
	//	bool ok = true;

	//	for (const auto& b : bindings)
	//	{
	//		auto it = name2idx.find(b.name);
	//		if (it == name2idx.end())
	//		{
	//			std::printf("Unknown cond '%.*s' for Axis '%s'\n",
	//				(int)b.name.size(), b.name.data(), ax.axisName.c_str());
	//			ok = false; 

	//			// 未知の条件名やズレ
	//			continue;
	//		}
	//		rt.conds.bind(CondID{ it->second }, b.fn);
	//		hit[it->second] = true;
	//	}

	//	// 漏れチェック
	//	for (std::uint32_t i = 0; i < hit.size(); ++i)
	//	{
	//		if (!hit[i])
	//		{
	//			std::printf("Cond '%s' not bound in Axis '%s'\n",
	//				ax.condOrder[i].c_str(), ax.axisName.c_str());
	//			ok = false;
	//		}
	//	}

	//	return ok;
	//}
}