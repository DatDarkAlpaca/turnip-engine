function detect_platform()
    filter { "system:Windows" }
        defines {
            "TUR_PLATFORM_WIN32",
            "VK_USE_PLATFORM_WIN32_KHR",
            "GLFW_STATIC",
            "GLFW_VULKAN_STATIC"
        }
        files {
            "src/platform/glfw/**.cpp",
            "src/platform/glfw/**.hpp",
        }
        links {
            "shcore.lib",
            "dxva2.lib"
        }
    filter { }

    filter { "system:Unix or linux" }
        defines {
            "TUR_PLATFORM_LINUX",
            "GLFW_STATIC",
            "GLFW_VULKAN_STATIC"
        }
        files {
            "src/platform/glfw/**.cpp",
            "src/platform/glfw/**.hpp",
        }
    filter { }
	
	filter { "system:macosx" }
        defines {
            "TUR_PLATFORM_MACOS",
            "GLFW_STATIC",
            "GLFW_VULKAN_STATIC"
        }
        files {
            "src/platform/glfw/**.cpp",
            "src/platform/glfw/**.hpp",
        }
    filter { }
end
