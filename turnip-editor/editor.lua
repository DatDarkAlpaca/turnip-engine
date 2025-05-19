create_project("turnip-editor", "ConsoleApp")
links { "turnip-engine" }
includedirs { 
	"%{wks.location}/turnip-engine/",
    "%{wks.location}/turnip-engine/src"
}

set_vendor_include_dirs()
detect_platform()
define_graphics_api()

libdirs { ".", "bin" }
links { "turnip-script.dll", "mono-2.0-sgen.dll" }

debugdir "%{wks.location}/"

externalwarnings "off"