project "turnip-engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

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
        "GLFW_VULKAN_STATIC",
    }

    includedirs {
        "%{prj.location}",
        "%{prj.location}/src"
    }

    -- Platform 
    DetectPlatform()

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
