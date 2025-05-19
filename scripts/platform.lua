function detect_platform()
    filter { "system:Windows" }
        defines {
            "TUR_PLATFORM_WIN32",
        }
    filter { }

    filter { "system:Unix or linux" }
        defines {
            "TUR_PLATFORM_LINUX",
        }
    filter { }
	
	filter { "system:macosx" }
        defines {
            "TUR_PLATFORM_MACOS",
        }
    filter { }
end

function setup_platform()
    filter { "system:Windows" }
        defines {
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
            "GLFW_STATIC",
            "GLFW_VULKAN_STATIC"
        }
        files {
            "src/platform/glfw/**.cpp",
            "src/platform/glfw/**.hpp",
        }
    filter { }
end

function initialize_platform_scripting()
    mono_sdk_sgen_file = path.join(os.getenv("MONO_SDK"), "bin", "mono-2.0-sgen.dll")

    filter "system:windows"
        postbuildcommands {
            "{COPYFILE} %[%{mono_sdk_sgen_file}] %[%{!wks.location}bin/mono-2.0-sgen.dll]",
        }
    filter {}
end
