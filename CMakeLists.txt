# Set the minimum required version of CMake for this project.
cmake_minimum_required (VERSION 3.11)

# Check for and set a specific policy regarding MSVC debug information format.
# This is particularly relevant for projects built with MSVC compilers.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Define the project name.
project ("comp4300-assignment-1")

# Find and load settings for SFML, ImGui-SFML, and yaml-cpp packages.
# These packages must be installed and discoverable by CMake.
find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)
find_package(yaml-cpp CONFIG REQUIRED)

# Create an executable target named 'comp4300-assignment-1' from specified source files.
add_executable(comp4300-assignment-1 "sources/main.cpp" "sources/assignment.h" )

# Link the created executable with the necessary libraries.
# These libraries are linked in a private scope (which affects their usage in terms of linking and visibility in case of your target being used by other targets)
target_link_libraries(comp4300-assignment-1 PRIVATE sfml-system sfml-graphics sfml-window ImGui-SFML::ImGui-SFML yaml-cpp)

# Conditional actions based on the build type.
# For Debug builds, create a symbolic link to the 'resources' directory.
# For non-Debug builds, copy the 'resources' directory.
if(CMAKE_BUILD_TYPE MATCHES Debug)
    message(STATUS "Creating symlink in ${CMAKE_BINARY_DIR} to ${CMAKE_SOURCE_DIR}/resources")
    file(CREATE_LINK ${CMAKE_SOURCE_DIR}/resources ${CMAKE_BINARY_DIR}/resources RESULT sym_res SYMBOLIC)
    message(STATUS "Symlink creation result: ${sym_res}")
else()
    message(STATUS "Copying ${CMAKE_SOURCE_DIR}/resources to ${CMAKE_BINARY_DIR}/resources")
    file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR})
endif()

# If using CMake version greater than 3.12, set C++ standard to C++20 for the target.
# This ensures the project uses modern C++ features.
if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET comp4300-assignment-1 PROPERTY CXX_STANDARD 20)
endif()
