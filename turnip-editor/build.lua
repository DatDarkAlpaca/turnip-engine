project "turnip-editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"

    links {
        "turnip-engine"
    }

    files {
        "src/**.cpp",
        "src/**.h"
    }

    defines {
        "GLFW_STATIC",
        "GLFW_VULKAN_STATIC"
    }
    
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
        "%{wks.location}/turnip-engine/",
        "%{wks.location}/turnip-engine/src",
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
