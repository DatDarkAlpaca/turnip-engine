project "turnip-editor"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    pchheader "pch.h"
    pchsource "src/pch.cpp"
    
    files {
        "src/**.cpp",
        "src/**.h"
    }
    
    includedirs {
        "%{prj.location}",
        "%{prj.location}/src",
        "%{wks.location}/turnip-engine/",
        "%{wks.location}/turnip-engine/src",
    }

    links {
        "turnip-engine"
    }

    -- Dependencies:
    set_vendor_include_dirs()
    set_vendor_defines()

    -- Platform
    detect_platform()

    -- Configurations
    filter "system:windows"
        systemversion "latest"

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
