#pragma once

#include "Engine/Log/Public/LogApi.hpp"

#include <string_view>
#include <iostream>

namespace Game::Log
{
	struct GameLogger : Engine::Log::ILogger
	{
		void Write(
			Engine::Log::Level level,
			std::string_view channel,
			std::string_view message
		) override
		{
			std::fprintf(
				stderr,
				"[Game][%.*s][%u] %.*s\n",
				(int)channel.size(), channel.data(),
				static_cast<unsigned>(level),
				(int)message.size(), message.data());;
		}
	};
}