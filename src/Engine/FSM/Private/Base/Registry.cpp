#include "Engine/FSM/Public/Base/Registry.hpp"

#include "Engine/FSM/Public/Base/Types.hpp"
#include "Engine/FSM/Public/Base/DTO.hpp"

#include <cstddef>
#include <string>
#include <cstdint>
#include <utility>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <unordered_map>

namespace Engine::FSM::Base
{
	void FSMRegistry::add(AxisDTO axis)
	{
		axes_.emplace_back(std::move(axis));
	}

	void FSMRegistry::add(FSMDTO fsm)
	{
		fsms_.emplace_back(std::move(fsm));
	}

	std::unordered_map<std::string, std::uint32_t>
		FSMRegistry::indexAxes(const std::vector<AxisDTO>& axes, BuildErrors& err)
	{
		std::unordered_map<std::string, std::uint32_t> map;
		map.reserve(axes.size());
		for (std::uint32_t i = 0; i < axes.size(); ++i)
		{
			const auto& name = axes[i].axis;
			if (name.empty()) {
				err.err("Axis name is empty at index " + std::to_string(i));
				continue;
			}
			if (!map.emplace(name, i).second) {
				err.err("Duplicate Axis name: " + name);
			}
		}
		return map;
	}

	BuildResult FSMRegistry::build(BuildStrictness policy) const
	{
		BuildErrors err;

		const auto axisIndex = indexAxes(axes_, err);

		// 辞書順で Axis の並びを決定 → 先頭から0..N-1を採番
		std::vector<std::uint32_t> order;
		order.reserve(axes_.size());
		for (std::uint32_t i = 0; i < axes_.size(); ++i)
			order.push_back(i);

		std::stable_sort(order.begin(), order.end(),
			[&](std::uint32_t a, std::uint32_t b)
			{
				return axes_[a].axis < axes_[b].axis;
			});

		// 
		std::vector<AxisTable> tables;
		tables.reserve(order.size());
		for (std::uint32_t rank = 0; rank < order.size(); ++rank)
		{
			const std::uint32_t i = order[rank];
			AxisTable t = makeAxisTables(axes_[i], err);
			t.id = AxisID{ rank };
			tables.emplace_back(std::move(t));
		}

		// Catalog skeleton 
		FSMCatalog cat = assembleCatalogSkeleton(tables);

		if (policy == BuildStrictness::Strict && !err.ok())
		{
			cat.axes.clear();
		}

		return BuildResult{ std::move(cat), std::move(err) };
	}

	AxisTable FSMRegistry::makeAxisTables(const AxisDTO& a, BuildErrors& err)
	{
		AxisTable t{};
		t.version = a.version;

		// 共通ヘルパ: ユニバース配列を検査→辞書順ユニーク→name→ID と order を埋める
		auto build_domain = [&](const char* kind,	// DTOの中身の種類
			const std::vector<std::string>& names,	// DTOの中身
			auto& outMap,							// 
			std::vector<std::string>& outOrder)
			{
				outMap.clear();
				outOrder.clear();
				outMap.reserve(names.size());
				outOrder.reserve(names.size());
				

				// 空名/重複チェック
				std::unordered_set<std::string> seen;
				seen.reserve(names.size());
				for (std::uint32_t i = 0; i < names.size(); ++i)
				{
					const auto& n = names[i];
					if (n.empty())
					{
						// error
						err.err(std::string("Axis '") + a.axis + "': " + kind +
							" contains empty name at index " + std::to_string(i));

						continue;
					}
					if (!seen.insert(n).second)// 追加 & 重複チェック
					{
						err.err(std::string("Axis '") + a.axis + "': " + kind +
							" duplicate name '" + n + "'");
					}
				}

				// ユニーク集合を辞書順に並べ替え
				std::vector<std::string> uniq;
				uniq.assign(seen.begin(), seen.end());
				std::sort(uniq.begin(), uniq.end());

				// 0..N-1 を割り当て (辞書順採番) し、mapとorder を埋める
				for (std::uint32_t i = 0; i < uniq.size(); ++i)
				{
					const std::string& n = uniq[i];

					//　StrongID は各 outMap の value型から推論
					using MapT = std::remove_reference_t<decltype(outMap)>;
					using IdT = typename MapT::mapped_type;
					outMap.emplace(n, IdT{ i });
					outOrder.push_back(n);
				}
			};

		build_domain("stateU", a.stateU, t.stateByName/*(string, ID)*/, t.stateOrder);
		build_domain("condU", a.condU, t.condByName, t.condOrder);
		build_domain("slotU", a.slotU, t.slotByName, t.slotOrder);
		build_domain("profileU", a.profileU, t.profileByName, t.profileOrder);

		return t;
	}

	FSMCatalog FSMRegistry::assembleCatalogSkeleton(const std::vector<AxisTable>& tables)
	{
		FSMCatalog cat;
		cat.axes.reserve(tables.size());

		for (const auto& t : tables)
		{
			CanonicalAxis ax{};
			ax.axis = t.id;			// AxisID をそのまま反映

			// ax.fsms はこの段階で空 (後でFSMを詰める)
			cat.axes.emplace_back(std::move(ax));
		}

		return cat;
	}

	static bool uniqNames(const std::vector<std::string>& names,
		const char* what, const std::string& owner, BuildErrors& err)
	{
		std::unordered_set<std::string> s;
		bool ok = true;
		for (size_t i = 0; i < names.size(); ++i)
		{
			if (names[i].empty())
			{
				err.err(owner + ": " + what + " contains empty name at index" + std::to_string(i));
				ok = false;
				continue;
			}
			if (!s.insert(names[i]).second)
			{
				err.err(owner + ": " + what + " duplicate name '" + names[i] + "'");
				ok = false;
			}
		}

		return ok;
	}

	bool FSMRegistry::validateFSM
	(
		const FSMDTO& fsm,
		const std::unordered_map<std::string, std::uint32_t>& axisNameToRank,
		const std::vector<AxisTable>& tables,
		BuildErrors& err,
		FSMCheck& out
	)
	{
		bool ok = true;
		const std::string owner = "FSM '" + fsm.fsm + "' (axis '" + fsm.axis + "')";

		// 
		auto ait = axisNameToRank.find(fsm.axis);
		if (ait == axisNameToRank.end())
		{
			err.err(owner + ": axis not found");
			return false;
		}
		out.axisRank = ait->second;
		const AxisTable& at = tables[out.axisRank];

		// states/profiles のチェック
		ok &= uniqNames(fsm.states, "states", owner, err);
		ok &= uniqNames(fsm.profiles, "profiles", owner, err);

		// states -> Axis stateU に存在するかチェック
		out.local2GlobalState.resize(fsm.states.size());
		for (size_t i = 0; i < fsm.states.size(); ++i)
		{
			auto it = at.stateByName.find(fsm.states[i]);
			if (it == at.stateByName.end())
			{
				err.err(owner + ": unknown state " + fsm.states[i] + "'");
				ok = false;
				continue;
			}

			out.local2GlobalState[i] = it->second;
		}

		// profiles -> Axis profileU 
		out.local2GlobalProfile.resize(fsm.profiles.size());
		for (size_t i = 0; i < fsm.profiles.size(); ++i)
		{
			auto it = at.profileByName.find(fsm.profiles[i]);
			if (it == at.profileByName.end())
			{
				err.err(owner + ": unknown profile '" + fsm.profiles[i] + "'");
				ok = false;
				continue;
			}
			out.local2GlobalProfile[i] = it->second;
		}

		// profile_defs の 重複名
		{
			std::unordered_set<std::string> seen;
			for (auto& d : fsm.profile_defs)
			{
				if (d.name.empty())
				{
					err.err(owner + ": profile_defs has empty name");
					ok = false;
					continue;
				}
				if (!seen.insert(d.name).second)
				{
					err.err(owner + ": duplicate profile_defs '" + d.name + "'");
					ok = false;
				}
			}
		}

		// transitions の from/ to/ slot 検証
		{
			std::unordered_set<std::string> stateSet(fsm.states.begin(), fsm.states.end());
			for (auto& tr : fsm.transitions)
			{
				if (!stateSet.count(tr.from))
				{
					err.err(owner + ": transition.from unknown '" + tr.from + "'");
					ok = false;
				}
				if (!stateSet.count(tr.to))
				{
					err.err(owner + ": transition.to unknown '" + tr.to + "'");
					ok = false;
				}
				if (at.slotByName.find(tr.slot) == at.slotByName.end())
				{
					err.err(owner + ": transition.slot unknown '" + tr.slot + "'");
					ok = false;
				}
			}
		}

		return ok;
	}
}