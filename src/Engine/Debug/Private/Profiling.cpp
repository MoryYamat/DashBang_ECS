#include "Engine/Debug/Public/ProfilingAPI.hpp"

#include "Engine/Log/Public/LogApi.hpp"

#include <unordered_map>
#include <sstream>
#include <string>
#include <iomanip>

#if ENGINE_ENABLE_PROFILING

namespace Engine::Debug::Profiling
{
	struct Stat
	{
		double total_us = 0.0;
		std::uint32_t calls = 0;
	};

	static std::unordered_map<std::string, Stat> g_stats;
	static std::uint32_t g_frameCounter = 0;
	static std::uint32_t g_printInterval = 60;

	void Profiler::SetPrintInterval(std::uint32_t frames)noexcept
	{
		g_printInterval = (frames == 0) ? 1u : frames;
	}

	void Profiler::Add(const char* name, double us)
	{
		auto& s = g_stats[std::string(name)];
		s.total_us += us;
		s.calls++;
	}

	void Profiler::EndFrame()
	{
		g_frameCounter++;

		// 
		if (g_frameCounter % g_printInterval != 0)
		{
			return;
		}

		// output
		for (const auto& [name, s] : g_stats)
		{
			if (s.calls == 0) continue;

			const double avg_us = s.total_us / static_cast<double>(s.calls);

			std::ostringstream oss;
			oss << name 
				<< " frame=" << g_frameCounter
				<< " interval=" << g_printInterval
				<< " " << name
				<< " calls=" << s.calls
				<< " avg=" << std::fixed << std::setprecision(2) << avg_us << "us"
				<< " total=" << std::fixed << std::setprecision(2) << s.total_us << "us";

			Engine::Log::Write(Engine::Log::Level::Info, "Profiler", oss.str());

		}

		// reset
		g_stats.clear();
	}
}

#else

namespace Engine::Debug::Profiling
{
	void Profiler::Add(const char*, double) {}
	void Profiler::EndFrame() {}
	void Profiler::SetPrintInterval(std::uint32_t) noexcept {}
}

#endif