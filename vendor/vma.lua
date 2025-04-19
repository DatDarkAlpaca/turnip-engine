project "vma"
	kind "StaticLib"
    warnings "off"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    systemversion "latest"

	targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

	includedirs {
		vendor_include_dirs["vulkan-headers"],
		vendor_include_dirs["vma"]
	}

    files {
		"VulkanMemoryAllocator/include/vk_mem_alloc*.h",
		"VulkanMemoryAllocator/src/Common.cpp",
		"VulkanMemoryAllocator/src/Common.h",
		"VulkanMemoryAllocator/src/VmaUsage.cpp",
		"VulkanMemoryAllocator/src/VmaUsage.h",
	}

	filter "system:linux"
		pic "On"
    filter { }

	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"
    filter {}

	filter "configurations:Release"
		runtime "Release"
		optimize "on"
        inlining "auto"
    filter { }