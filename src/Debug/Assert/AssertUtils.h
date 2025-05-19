// アサーションシステム
// assert system
#pragma once

#include <iostream>
#include <cstdlib>
#include <source_location>

namespace DebugUtils
{
	inline void AssertFial(const char* expr,
		const std::source_location location = std::source_location::current())
	{
		std::cerr << "[ASSERTION FAILED]\n"
			<< "Expression : " << expr << "\n"
			<< "Function : " << location.function_name() << "\n"
			<< "File : " << location.file_name() << ":" << location.line() << "\n";
		std::abort();
	}

#define ASSERT(expr) \
		((expr) ? (void)0 : ::Debug::AssertFail(#expr, std::source_location::current()))

}