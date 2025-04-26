[![Codacy Badge](https://app.codacy.com/project/badge/Grade/ec020ba40c7b4754ab2975940e76f178)](https://app.codacy.com/gh/DatDarkAlpaca/turnip-engine/dashboard?utm_source=gh&utm_medium=referral&utm_content=&utm_campaign=Badge_grade)
[![Average time to resolve an issue](http://isitmaintained.com/badge/resolution/DatDarkAlpaca/turnip-engine.svg)](http://isitmaintained.com/project/DatDarkAlpaca/turnip-engine "Average time to resolve an issue")
[![Percentage of issues still open](http://isitmaintained.com/badge/open/DatDarkAlpaca/turnip-engine.svg)](http://isitmaintained.com/project/DatDarkAlpaca/turnip-engine "Percentage of issues still open")

# Turnip Engine

> Turnip is my toy game engine that follows the principles of the "yet another" mentality.

# Getting Started

Turnip doesn't have prebuilt binaries, so if you want to use it, you'll need to build it yourself.
It uses [Premake 5](https://github.com/premake/premake-core/releases) to generate project files, but it has only been tested with [Visual Studio](https://visualstudio.microsoft.com/pt-br/downloads/).

## Prerequisites

To build this project, you'll need to install the following:

* [Python 3](https://www.python.org/downloads/)
* [Conan 2.0](https://conan.io/downloads)
* [Vulkan](https://vulkan.lunarg.com/#new_tab)
* [Premake 5](https://github.com/premake/premake-core/releases)

You may need to set these environment variables if they aren't automatically detected:
1. **Vulkan**:
    Set `VULKAN_SDK` to point to your Vulkan SDK installation (e.g., C:/Vulkan/<version>).
2. **Mono**:
    Likewise, set `MONO_SDK` to point to your Mono installation directory (e.g., C:/Program Files (x86)/Mono).
3. **Premake**:
    Ensure the directory containing premake5 is added to your system's PATH.


If the SDK fails to include it automatically, you must provide an environment variable named VULKAN_SDK under `VulkanSDK/<version>`. Additionally, you may need to add an environment variable to use premake5.

## Installation

1. **Clone the repository**
    ```bash
    git clone --recurse-submodules https://github.com/DatDarkAlpaca/turnip-engine
    ```

2. **Generate the project**
    If you're building on a Windows machine, you can use the build.cmd file to execute the Python script. This automatically calls premake, so you must specify the project file of your choice.

    ```bash
    .\build <action: vs2022, vs2021, gmake, ...>
    ```

3. **Compile the project**
   Use the generated project files to build the project. After that, you should have a working copy under `turnip-engine/build/bin/{configuration}/turnip-editor`.)

# License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT) - see the [LICENSE](LICENSE) file for details
