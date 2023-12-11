cmake_minimum_required (VERSION 3.11)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("comp4300-assignment-1")

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)
find_package(ImGui-SFML CONFIG REQUIRED)

add_executable (comp4300-assignment-1 "sources/comp4300-assignment-1.cpp" "sources/comp4300-assignment-1.h")

target_link_libraries(comp4300-assignment-1 PRIVATE sfml-system sfml-graphics sfml-window)
target_link_libraries(comp4300-assignment-1 PRIVATE ImGui-SFML::ImGui-SFML)

# Copy resources to build directory (TODO: ideally we should create a symlink instead when in debug mode, and copy in release mode)
message(STATUS "Copying ${CMAKE_SOURCE_DIR}/resources to ${CMAKE_BINARY_DIR}/resources")
file(COPY ${CMAKE_SOURCE_DIR}/resources DESTINATION ${CMAKE_BINARY_DIR}) 

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET comp4300-assignment-1 PROPERTY CXX_STANDARD 20)
endif()
