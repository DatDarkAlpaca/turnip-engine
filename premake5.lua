require "scripts/clear"
include "dependencies/conandeps.premake5.lua"

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

include "turnip-engine/build.lua"
include "turnip-editor/build.lua"