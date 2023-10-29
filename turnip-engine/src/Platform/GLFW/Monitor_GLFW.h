#pragma once
#include <GLFW/glfw3.h>

#include "Common.h"
#include "Core/Window/MonitorData.h"

namespace tur
{
	class Monitor
	{
	public:
		static uint64_t GetMonitorAmount()
		{
			int count;
			GLFWmonitor** _ = glfwGetMonitors(&count);

			if (!count)
				TUR_LOG_WARN("Failed to fetch monitor data.");

			return static_cast<uint64_t>(count);
		}
	
		static inline GLFWmonitor* GetMonitorPointer(uint64_t index)
		{
			if (GetMonitorAmount() <= index)
				TUR_LOG_CRITICAL("Invalid monitor index selected: {}.", index);
			
			int count = 0;
			GLFWmonitor* glfwMonitor = glfwGetMonitors(&count)[index];
			return glfwMonitor;
		}

		static inline GLFWmonitor* GetPrimaryMonitorPointer()
		{
			return glfwGetPrimaryMonitor();
		}

		static inline MonitorData FetchMonitorData(uint64_t index)
		{
			if (GetMonitorAmount() <= index)
				TUR_LOG_CRITICAL("Invalid monitor index selected: {}.", index);
			
			return ParseMonitorData(GetMonitorPointer(index));
		}

		static MonitorData FetchPrimaryMonitor()
		{
			return ParseMonitorData(GetPrimaryMonitorPointer());
		}

	private:
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