require "scripts/clear"
require "scripts/project"
require "scripts/graphics"
require "scripts/platform"
require "vendor/dependencies"

workspace "turnip-engine"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

    startproject "turnip-editor"

    flags {
		"MultiProcessorCompile"
	}

output_path         = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binaries_path       = "%{wks.location}/build/bin/" .. "%{output_path}"
intermediate_path   = "%{wks.location}/build/intermediate/" .. "%{output_path}"
vendor_path         = "%{wks.location}/vendor/"

group "dependencies"
    include "vendor/glfw.lua"
    include "vendor/glad.lua"
    include "vendor/imgui.lua"
    include "vendor/vma.lua"

    if os.host("windows") then
        include "vendor/imgui-glfw.lua"
    end

    include "vendor/imgui-opengl.lua"
group ""

include "turnip-engine/engine.lua"
include "turnip-editor/editor.lua"