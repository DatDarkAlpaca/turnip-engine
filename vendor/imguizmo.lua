project "imguizmo"
    kind "StaticLib"
    language "C"
    warnings "off"
    staticruntime "off"
    systemversion "latest"
    
    targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    includedirs {
        vendor_include_dirs["imgui"],
    }

    files {
        "imguizmo/*.h",
        "imguizmo/*.cpp"
    }

    includedirs {
        "imguizmo/"
    }
    
    filter "configurations:Debug"
        runtime "Debug"
        symbols "on"
    filter { }
    
    filter "configurations:Release"
        runtime "Release"
        optimize "on"
        inlining "auto"
    filter { }
