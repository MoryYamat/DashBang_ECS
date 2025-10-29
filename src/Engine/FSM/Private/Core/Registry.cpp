#include "Engine/FSM/Public/Core/Registry.hpp"

#include "Engine/FSM/Public/Core/Types.hpp"

#include <cstdint>
#include <vector>
#include <string>
#include <string_view>
#include <unordered_set>
#include <algorithm>
#include <numeric>


namespace Engine::FSM::Core
{
	// 形式チェック (重複など)
	void FSMRegistry::add(AuthoringFSM def)
	{
		auto dedup = [this](const std::vector<std::string>& v, std::string_view kind)
			{
				std::unordered_set<std::string> s;
				for (auto& x : v)
				{
					if (!s.insert(x).second)
					{
						issues_.emplace_back(
							Kind::DuplicateTransition,
							std::string(kind) + "duplicated: " + x
							);
						}
				}
			};

		dedup(def.states, "state");
		dedup(def.conds, "cond");

		const std::string axis = def.axis;
		const uint32_t ver = def.version;

		// 新規追加
		auto it = byAxis_.find(axis);
		if (it == byAxis_.end())
		{
			byAxis_.emplace(axis, AuthoringSlot{ std::move(def), ver });
			return;
		}

		// 重複が存在
		// verが新しい→上書き
		auto& slot = it->second;
		if (ver > slot.version)
		{
			issues_.emplace_back(
				Kind::DuplicateTransition,
				"Axis '" + axis + " ' replaced: version " +
				std::to_string(slot.version) + " -> " + std::to_string(ver)
				);
			slot.def = std::move(def);
			slot.version = ver;
		}
		else
		{
			// verが古い
			issues_.emplace_back(
					Kind::DuplicateTransition,
					"Axis '" + axis + "' ignored: incoming version" +
					std::to_string(ver) + " <= existing" + std::to_string(slot.version)
				);
		}
	}

	// namesを辞書順ユニーク、name->indexを作る
	static void buildNameTable(
		const std::vector<std::string>& in,
		std::vector<std::string>& outSortedUnique,
		std::unordered_map<std::string, std::uint32_t>& name2idx
	)
	{
		outSortedUnique = in;
		std::sort(outSortedUnique.begin(), outSortedUnique.end());
		outSortedUnique.erase(std::unique(outSortedUnique.begin(), outSortedUnique.end()), outSortedUnique.end());//重複削除

		name2idx.clear();
		name2idx.reserve(outSortedUnique.size());
		for (std::uint32_t i = 0; i < outSortedUnique.size(); ++i)
		{
			name2idx.emplace(outSortedUnique[i], i);
		}
	}

	// 文字列→決定的ID付きテーブル(CSR)に正規化
	std::vector<CanonicalAxis> FSMRegistry::build()
	{
		std::vector<CanonicalAxis> out;

		// 軸名を辞書順にソート
		std::vector<const AuthoringSlot*> slots;
		slots.reserve(byAxis_.size());
		for (auto& kv : byAxis_) slots.push_back(&kv.second);
		std::sort(slots.begin(), slots.end(),
			[](const AuthoringSlot* a, const AuthoringSlot* b)
			{
				return a->def.axis < b->def.axis;
			});

		// 正規化(軸ごと)
		for (const AuthoringSlot* slot : slots)
		{
			const auto& def = slot->def;

			std::vector<std::string> stateNames;
			std::vector<std::string> condNames;
			std::unordered_map<std::string, std::uint32_t> state2idx, cond2idx;// name→indexマップ
			buildNameTable(def.states, stateNames, state2idx);// 辞書順で重複を除去 + (name→indexマップ) を作成
			buildNameTable(def.conds, condNames, cond2idx);   // 人間の可読名→StateIDを一意

			const std::uint32_t stateCount = static_cast<std::uint32_t>(stateNames.size());
			if (stateCount == 0)
			{
				issues_.emplace_back(Kind::MissingState,
					"Axis '" + def.axis + "': no states defined"
				);
				continue;
			}

			// 文字列遷移 → ID化 (未定義はIssueに積んでスキップ)
			struct Row { std::uint32_t from; Transition tr; std::uint32_t seq; };
			std::vector<Row> rows;
			rows.reserve(def.transitions.size());

			std::uint32_t seq = 0;
			for (const auto& at : def.transitions)
			{
				auto itFrom = state2idx.find(at.from);
				auto itTo = state2idx.find(at.to);
				auto itCond = cond2idx.find(at.cond);

				bool ok = true;// 存在確認
				if (itFrom == state2idx.end())
				{
					issues_.emplace_back(
						Kind::MissingState,
						"Axis '" + def.axis + "': from state not found: " + at.from
					);
					ok = false;
				}
				if (itTo == state2idx.end())
				{
					issues_.emplace_back(
						Kind::MissingState,
						"Axis '" + def.axis + "' to state not found: " + at.to
					);
					ok = false;
				}
				if (itCond == cond2idx.end())
				{
					issues_.emplace_back(
						Kind::MissingCond,
						"Axis '" + def.axis + "': cond not found: " + at.cond
					);
					ok = false;
				}
				if (!ok)
				{
					++seq;
					continue;
				}

				// すべての定義が正常の場合、IDデータ(index)を追加
				Transition tr{};
				tr.to = StateID{ itTo->second };
				tr.cond = CondID{ itCond->second };
				tr.prio = at.prio;
				tr.flags = 0;

				rows.push_back(Row{ itFrom->second, tr, seq++ });
			}

			// 遷移組のソート→(from昇順, prio降順, 入力順)
			std::stable_sort(rows.begin(), rows.end(),
				[](const Row& a, const Row& b)
				{
					if (a.from != b.from) return a.from < b.from;
					if (a.tr.prio != b.tr.prio) return a.tr.prio > b.tr.prio;// 高優先度先
					return a.seq < b.seq;// 入力順
				});

			// CSR(Compressed Sparse Row)を作成
			// headIndex[i]で遷移群の開始位置を取得
			// -> edges(遷移群) を取得する
			CanonicalAxis ca{};
			ca.axisName = def.axis;
			ca.schemaVersion = slot->version;
			ca.stateNames = stateNames;
			ca.condNames = condNames;

			ca.stateIDs.resize(stateCount);
			for (std::uint32_t i = 0; i < stateCount; ++i) ca.stateIDs[i] = StateID{ i };

			ca.headIndex.assign(stateCount + 1, 0);
			ca.edges.reserve(rows.size());// サイズ確保

			// headIndex を作る ヒストグラム→累積和
			for (const auto& r : rows) ++ca.headIndex[r.from + 1];
			for (std::uint32_t i = 1; i <= stateCount; ++i) ca.headIndex[i] += ca.headIndex[i - 1];

			// edges を詰めなおす (全順序で不要だが念のため)
			ca.edges.resize(rows.size());
			{
				std::vector<std::uint32_t> cursor = ca.headIndex;
				for (const auto& r : rows)
				{
					ca.edges[cursor[r.from]++] = r.tr;
				}
			}

			out.push_back(std::move(ca));
		}

		//
		for (std::uint16_t i = 0; i < out.size(); ++i)
		{
			out[i].axis = AxisID{ i };
		}

		return out;
	}
}// namespace