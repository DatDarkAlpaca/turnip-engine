create_project("turnip-testing", "ConsoleApp")
links { "turnip-engine" }
includedirs { 
	"%{wks.location}/turnip-engine/",
    "%{wks.location}/turnip-engine/src"
}

libdirs { ".", "bin" }
links { "turnip-script.dll", "mono-2.0-sgen.dll" }

debugdir "%{wks.location}"

externalwarnings "off"