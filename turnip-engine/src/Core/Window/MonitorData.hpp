#pragma once
#include "Common.hpp"
#include <string>

namespace tur
{
	struct MonitorData
	{
		uint64_t physicalWidth = 0, physicalHeight = 0;
		uint64_t workWidth = 0, workHeight = 0, workX = 0, workY = 0;
		float contentScaleX = 1.f, contentScaleY = 1.f;
		std::string monitorName;
	};
}