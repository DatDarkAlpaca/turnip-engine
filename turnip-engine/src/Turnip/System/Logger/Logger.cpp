#include "pch.h"
#include "Logger.h"

namespace tur
{
	void initialize_logger()
	{
#ifdef TUR_DEBUG
		spdlog::set_level(spdlog::level::debug);
#endif
	}
}