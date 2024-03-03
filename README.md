# Turnip Engine

[![Codacy Badge](https://api.codacy.com/project/badge/Grade/4237c333db9c4412a73fadfa2de405aa)](https://app.codacy.com/gh/DatDarkAlpaca/turnip-engine?utm_source=github.com&utm_medium=referral&utm_content=DatDarkAlpaca/turnip-engine&utm_campaign=Badge_Grade)

> Turnip is my toy game engine that follows the principles of the "yet another" mentality.

# Getting Started

Turnip doesn't have prebuilt binaries, so if you want to use it, you'll need to build it yourself.
It uses [Premake 5](https://github.com/premake/premake-core/releases) to generate project files, but it has only been tested with [Visual Studio](https://visualstudio.microsoft.com/pt-br/downloads/).

## Prerequisites

You'll need to install `python`, `conan` and `premake5` in order to build this project.

* [Python 3](https://www.python.org/downloads/)
* [Conan 2.0](https://conan.io/downloads)
* [Premake 5](https://github.com/premake/premake-core/releases)

You may also need to add an environment variable to use `premake5`.

## Installation
1. **Clone the repository**
    ```bash
    git clone https://github.com/DatDarkAlpaca/turnip-engine
    ```

2. **Generate the project**
    If you're building on a Windows machine, you can use the `build.cmd` file to execute the Python script. 
    This automatically calls premake, so you must specify the project file of your choice.

    ```bash
    .\build <action: vs2022, vs2021, gmake, ...>
    ```

3. **Compile the project**
   Use the generated project files to build the project. 
   After that, you should have a working copy under `turnip-engine/build/bin/{configuration}/turnip-editor`. 

# Resources
An incomplete list of resources that I've used to build this engine so far.

- [Learn OpenGL](https://learnopengl.com)
- [Game Engine Architecture, 3rd Edition](https://www.amazon.com/Engine-Architecture-Third-Jason-Gregory/dp/1138035459)
- [Cherno's Game Engine Playlist](https://www.youtube.com/watch?v=JxIZbV_XjAs&list=PLlrATfBNZ98dC-V-N3m0Go4deliWHPFwT)
- [Progrematic's Game Engine Playlist](https://www.youtube.com/watch?v=wMk182vENjk&list=PL7lh9ryRNHSIzqKzEdYPG94B0uvfqhHpb)
- [OpenGL with Python](https://www.youtube.com/watch?v=LCK1qdp_HhQ&list=PLn3eTxaOtL2PDnEVNwOgZFm5xYPr4dUoR)
- [Chili's C++ 3D DirectX Tutorial](https://www.youtube.com/watch?v=_4FArgOX1I4&list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD)
- [OpenGL Superbible, 4th Edition](https://www.amazon.com/OpenGL-SuperBible-Comprehensive-Tutorial-Reference/dp/0321498828)
- [SFML Game Development](https://www.amazon.com/SFML-Game-Development-Jan-Haller/dp/1849696845)
- [Game Engine Design and Implementation](https://www.amazon.com.br/Game-Engine-Design-Implementation-Foundations/dp/0763784516)
- [FGED I](https://foundationsofgameenginedev.com/#fged1)
- [FGED II](https://foundationsofgameenginedev.com/#fged2)
- [Vulkan, GetIntoGameDev](https://www.youtube.com/watch?v=W2I0DofOw9M&list=PLn3eTxaOtL2NH5nbPHMK7gE07SqhcAjmk&index=1)

# License

This project is licensed under the [MIT License](https://opensource.org/licenses/MIT) - see the [LICENSE](LICENSE) file for details
