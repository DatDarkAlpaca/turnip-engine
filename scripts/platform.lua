function DetectPlatform()
    filter { "system:Windows" }
        defines {
            -- "TUR_WINDOWING_WIN32",
            "TUR_WINDOWING_GLFW",
            "TUR_PLATFORM_WIN32",
            "VK_USE_PLATFORM_WIN32_KHR"
        }
        files {
            "src/Platform/GLFW/**.cpp",
            "src/Platform/GLFW/**.h",
            --"src/Platform/WIN32/**.cpp",
            --"src/Platform/WIN32/**.h",
        }
        links {
            "shcore.lib",
            "dxva2.lib"
        }
    filter { }

    filter { "system:Unix or linux or macosx" }
        defines {
            "TUR_WINDOWING_GLFW",
            "TUR_PLATFORM_GLFW",
        }
        files {
            "src/Platform/GLFW/**.cpp",
            "src/Platform/GLFW/**.h",
        }
    filter { }
end   