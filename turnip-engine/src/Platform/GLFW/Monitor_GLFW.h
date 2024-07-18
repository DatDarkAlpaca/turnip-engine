#pragma once
#include "Common.h"
#include "Core/Window/MonitorData.h"

#include <GLFW/glfw3.h>

namespace tur::platform
{
	class Monitor
	{
	public:
		static uint32_t GetMonitorAmount()
		{
			int count = 0;
			GLFWmonitor** _ = glfwGetMonitors(&count);

			if (!count)
				TUR_LOG_WARN("Failed to fetch monitor data.");

			return static_cast<uint32_t>(count);
		}
	
		static inline MonitorData FetchMonitorData(uint32_t index)
		{
			if (GetMonitorAmount() <= index)
			{
				TUR_LOG_ERROR("Invalid monitor index selected: {}.", index);
				return { };
			}
			
			return ParseMonitorData(GetMonitorPointer(index));
		}

	private:
		static inline GLFWmonitor* GetMonitorPointer(uint32_t index)
		{
			if (GetMonitorAmount() <= index)
			{
				TUR_LOG_ERROR("Invalid monitor index selected: {}.", index);
				return nullptr;
			}

			int count = 0;
			GLFWmonitor* glfwMonitor = glfwGetMonitors(&count)[index];
			return glfwMonitor;
		}

		static inline MonitorData ParseMonitorData(GLFWmonitor* glfwMonitor)
		{
			MonitorData monitorData;

			{
				int physicalWidth, physicalHeight;
				glfwGetMonitorPhysicalSize(glfwMonitor, &physicalWidth, &physicalHeight);
				monitorData.physicalWidth = physicalWidth;
				monitorData.physicalHeight = physicalHeight;
			}

			{
				float xScale, yScale;
				glfwGetMonitorContentScale(glfwMonitor, &xScale, &yScale);
				monitorData.contentScaleX = xScale;
				monitorData.contentScaleY = yScale;
			}

			{
				int xPos, yPos, width, height;
				glfwGetMonitorWorkarea(glfwMonitor, &xPos, &yPos, &width, &height);

				monitorData.workWidth = width;
				monitorData.workHeight = height;
				monitorData.workX = xPos;
				monitorData.workY = yPos;
			}

			{
				const char* name = glfwGetMonitorName(glfwMonitor);
				monitorData.monitorName = name;
			}

			return monitorData;
		}
	};
}