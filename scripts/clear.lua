premake.modules.clear = {}
local m = premake.modules.clear
local p = premake

newaction {
    trigger = "clear",
    description = "Clears generated project files and binaries",

    execute = function()
        os.rmdir("./build")
        os.rmdir("./bin")
        os.rmdir("./intermediate")
        os.rmdir("./.vs")
        os.remove('*.sln')
        os.remove('turnip-editor/**.sln')
        os.remove('turnip-editor/**.vcxproj')
        os.remove('turnip-editor/**.vcxproj.filters')
        os.remove("turnip-editor/**.vcxproj.user")
        os.remove('turnip-engine/**.sln')
        os.remove('turnip-engine/**.vcxproj')
        os.remove('turnip-engine/**.vcxproj.filters')
        os.remove("turnip-engine/**.vcxproj.user")
        os.remove('**.props')
        os.remove('**.targets')
        os.remove('**.Debug')
        os.remove('**.Release')
        os.remove('**.log')
        os.remove('**.ini')
    end
}

return m
