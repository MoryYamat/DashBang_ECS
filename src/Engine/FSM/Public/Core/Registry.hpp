#pragma once

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Authoring.hpp"

#include <vector>
#include <string>
#include <string_view>
#include <unordered_map>

namespace Engine::FSM::Core
{


	enum class Kind
	{
		DuplicateTransition, MissingState, MissingCond
	};

	// 検証
	struct Issue
	{
		Kind kind;

		std::string msg;

		Issue(Kind k) : kind(k) {};
		Issue(Kind k, std::string error) : kind(k) , msg(error){};
	};

	struct AuthoringSlot
	{
		AuthoringFSM def;
		uint32_t version;
	};

	// TODO: 本当はAuthoringTransitionsを書くだけで成立するようにしたい
	class FSMRegistry
	{
	private:
		std::unordered_map<std::string, AuthoringSlot> byAxis_;
		std::vector<Issue>issues_;

	public:
		void add(AuthoringFSM def);


		// 正規化→凍結
		// 同名Axisの多重定義は version で衝突解決 or エラー
		std::vector<CanonicalAxis> build();

		// 逆引き / デバッグ
		const CanonicalAxis* findAxis(AxisID axisId) const;
		const CanonicalAxis* findAxisByName(std::string_view axisName) const;



		const std::vector<Issue>& issues() const;
	};
}