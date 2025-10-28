#pragma once

#include "Engine/FSM/Public/Core/Types.hpp"
#include "Engine/FSM/Public/Core/Authoring.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace Engine::FSM::Core
{
	struct AuthoringTransition
	{
		std::string from;
		std::string to;
		std::string cond;
		std::uint8_t prio;
	};

	struct AuthoringFSM
	{
		std::string axis;
		std::vector<std::string> states;
		std::vector<std::string> conds;
		std::vector<AuthoringTransition> transitions;
		std::uint32_t version = 1;// 更新衝突を回避するための識別番号
	};



}