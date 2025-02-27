vendor_include_dirs = { }
vendor_include_dirs["entt"]             = "%{vendor_path}/entt/single_include"
vendor_include_dirs["glad"]             = "%{vendor_path}/glad-premake/include"
vendor_include_dirs["glfw"]             = "%{vendor_path}/glfw/include"
vendor_include_dirs["glm"]              = "%{vendor_path}/glm"
vendor_include_dirs["imgui"]            = "%{vendor_path}/imgui"
vendor_include_dirs["imgui-glfw"]       = "%{vendor_path}/imgui/backends"
vendor_include_dirs["imgui-opengl"]     = "%{vendor_path}/imgui/backends"
vendor_include_dirs["imgui-vulkan"]     = "%{vendor_path}/imgui/backends"
vendor_include_dirs["spdlog"]           = "%{vendor_path}/spdlog/include"
vendor_include_dirs["vulkan-headers"]   = "%{vendor_path}/Vulkan-Headers/include"
vendor_include_dirs["vma"]              = "%{vendor_path}/VulkanMemoryAllocator/include"
vendor_include_dirs["nlohmann"]         = "%{vendor_path}/json/single_include"
vendor_include_dirs["stb"]         		= "%{vendor_path}/stb"

vendor_lib_dirs = { }
vendor_lib_dirs["vulkan"]         		= os.getenv("VULKAN_SDK") .. '/Lib/'

vendor_links = {}
vendor_links[0] = "glad"
vendor_links[1] = "glfw"
vendor_links[2] = "imgui"
vendor_links[3] = "imgui-glfw"
vendor_links[4] = "imgui-opengl"
vendor_links[4] = "imgui-vulkan"
vendor_links[5] = "vma"
vendor_links[6] = "vulkan-1.lib"

vendor_defines = {}
vendor_defines[0] = "GLFW_INCLUDE_NONE"

function set_vendor_include_dirs()
    for vendor_names, vendor_paths in pairs(vendor_include_dirs) do
        includedirs { vendor_paths }
    end
end

function set_vendor_lib_dirs()
    for vendor_names, vendor_paths in pairs(vendor_lib_dirs) do
        libdirs { vendor_paths }
    end
end

function set_vendor_links()
    for i, vendor_name in pairs(vendor_links) do
        links { vendor_name }
    end
end

function set_vendor_defines()
    for i, vendor_define in pairs(vendor_defines) do
        defines { vendor_define }
    end
end

function setup_vendors()
    set_vendor_include_dirs()
	set_vendor_lib_dirs()
	set_vendor_links()
    set_vendor_defines()
end