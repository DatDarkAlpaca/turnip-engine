project "imgui-opengl"
	kind "StaticLib"
    warnings "off"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"
    systemversion "latest"

	targetdir(binaries_path .. "/%{prj.name}")
    objdir(intermediate_path .. "/%{prj.name}")

    includedirs {
        vendor_include_dirs["imgui"],
    }

    files {
		"imgui/backends/imgui_impl_opengl3.h",
		"imgui/backends/imgui_impl_opengl3.cpp",
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