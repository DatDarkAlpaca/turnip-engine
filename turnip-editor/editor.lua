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
    
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
        "%{wks.location}/turnip-engine/",
        "%{wks.location}/turnip-engine/src",
        "%{vendor_path}/imgui",
        "%{vendor_path}/imgui/backends",
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
