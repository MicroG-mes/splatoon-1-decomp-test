cmake_minimum_required(VERSION 3.15)
project(splatoon_decomp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files (library actors and subsystems)
file(GLOB_RECURSE LIB_SOURCES
    "src/Game/*.cpp"
    "src/cafe/*.cpp"
    "src/sead/*.cpp"
)

# Build as a static library
add_library(splatoon_lib STATIC ${LIB_SOURCES})

# Target-specific compiler options
if(MSVC)
    target_compile_definitions(splatoon_lib PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(splatoon_lib PRIVATE /W3 /Zc:__cplusplus /std:c++17)
else()
    target_compile_options(splatoon_lib PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Native PC Host Executable
add_executable(Gambit "src/Main/main_pc.cpp")
target_include_directories(Gambit PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
    target_link_libraries(splatoon_lib PRIVATE d3d11 dxgi d3dcompiler)
    target_link_libraries(Gambit PRIVATE splatoon_lib d3d11 dxgi d3dcompiler)
    target_compile_definitions(Gambit PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_compile_options(Gambit PRIVATE /W3 /Zc:__cplusplus /std:c++17)
else()
    target_link_libraries(Gambit PRIVATE splatoon_lib)
endif()
