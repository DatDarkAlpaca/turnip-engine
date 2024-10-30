#pragma once
#include "Graphics/CommandBuffer.hpp"
#include "Graphics/GraphicsLayer.hpp"
#include "CommonVulkan.hpp"

#include "VulkanInitializer.hpp"
#include "Objects/Objects.hpp"

#include "Platform/Platform.hpp"

namespace tur::vulkan
{
	class GraphicsLayerVulkan final : public IGraphicsLayer
	{
	public:
		explicit GraphicsLayerVulkan(platform::Window& window)
			: r_Window(window)
		{

		}

	public:
		void Initialize() override
		{

		};

		void Shutdown() override
		{

		};

	public:
		void SetInstanceObject(const InstanceObject& object) { instanceObject = object; }
		void SetSurfaceObject(const SurfaceObject& object) { surfaceObject = object; }
		void SetPhysicalDeviceObject(const PhysicalDeviceObject& object) { physicalDeviceObject = object; }
		void SetLogicalDeviceObject(const LogicalDeviceObject& object) { logicalDeviceObject = object; }
		void SetAllocator(const VmaAllocator& allocator) { vmaAllocator = allocator; }
		void SetSwapchainObject(const SwapchainObject& object) { swapchainObject = object; }
		void SetFrames(const Frames& frames) { this->frames = frames; }

	private:
		platform::Window& r_Window;

	public:
		InstanceObject instanceObject;
		SurfaceObject surfaceObject;
		PhysicalDeviceObject physicalDeviceObject;
		LogicalDeviceObject logicalDeviceObject;
		DeviceQueues deviceQueues;
		VmaAllocator vmaAllocator;
		SwapchainObject swapchainObject;
		Frames frames;
	};
}