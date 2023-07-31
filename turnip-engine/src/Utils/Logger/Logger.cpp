#include "pch.h"
#include "Logger.h"

namespace tur
{
	void InitializeLogger()
	{
#ifdef TUR_DEBUG
		spdlog::set_level(spdlog::level::debug);
#endif
	}
}