# COMP 4300 - Fall 2023 - C++ Game Programming
## Assignment 1

### Tools
- [Visual Studio 2022](https://visualstudio.microsoft.com/vs/)
- [vcpk C/C++ package manager](https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/)
- [CMake](https://cmake.org/)

### Libraries
- [SFML - 2.5.1](https://github.com/SFML/SFML)
- [imgui-sfml](https://github.com/SFML/imgui-sfml)
- [imgui](https://github.com/ocornut/imgui)
- [yaml-cpp](https://github.com/jbeder/yaml-cpp)

### Setup

1. Download [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) (make sure to select the [necessary components from the installer](https://devblogs.microsoft.com/cppblog/vcpkg-is-now-included-with-visual-studio/#:~:text=You%20can%20also%20find%20it%20in%20the%20installer%20by%20searching%20for%20vcpkg%20package%20manager%20under%20the%20Individual%20components%20tab.))
2. Clone repository ```git clone https://github.com/JorgeMag96/comp4300-assignment-1.git``` and open in Visual Studio 2022
4. Run ```vcpk install``` (more info on what this command is doing can be found [here](https://learn.microsoft.com/en-us/vcpkg/commands/install))
5. Update ```CMAKE_TOOLCHAIN_FILE``` value in [CMakePresets.json](./CMakePresets.json#L13) with output from previous command (what's this variable for ? [find more information here](https://cmake.org/cmake/help/latest/variable/CMAKE_TOOLCHAIN_FILE.html))

For more information on this project structure, you can reference this [blog post](https://learn.microsoft.com/en-us/vcpkg/users/buildsystems/cmake-integration)
