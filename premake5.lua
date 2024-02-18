require "scripts/clear"
include "scripts/platform"
include "vendor/dependencies/conandeps.premake5.lua"

workspace "turnip-engine"
    architecture "x64"
    configurations {
        "Debug",
        "Release"
    }

    startproject "turnip-editor"

output_path         = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
binaries_path       = "%{wks.location}/build/bin/" .. "%{output_path}"
intermediate_path   = "%{wks.location}/build/intermediate/" .. "%{output_path}"
vendor_path         = "%{wks.location}/vendor/"

include "vendor/imgui.lua"
include "turnip-engine/engine.lua"
include "turnip-editor/editor.lua"