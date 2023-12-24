project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    links {
        "shcore.lib",
        "dxva2.lib"
    }

    files {
        "src/Core/**.cpp",
        "src/Core/**.h",
        "src/Graphics/**.cpp",
        "src/Graphics/**.h",
        "src/Util/**.cpp",
        "src/Util/**.h",
        "src/Platform/*.h",
        "src/*.h",
        "src/*.cpp",
    }
   
    defines {
        "GLFW_STATIC",
        "GLFW_VULKAN_STATIC"
    }

    includedirs {
        "%{prj.location}",
        "%{prj.location}/src"
    }

    -- Platform 
    defines {
        "TUR_WINDOWING_WIN32",
        "TUR_PLATFORM_WIN32",
    }

    files {
        "src/Platform/WIN32/**.cpp",
        "src/Platform/WIN32/**.h",
    }

    -- Configurations
    filter { "configurations:Debug" }
        runtime "Debug"
        symbols "on"
        defines { "TUR_DEBUG" }
    filter { }
        
    filter { "configurations:Release" }
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }

    conan_setup()
