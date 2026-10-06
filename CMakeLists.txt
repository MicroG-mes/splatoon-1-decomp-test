cmake_minimum_required(VERSION 3.15)
project(splatoon_decomp CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Source files
file(GLOB_RECURSE SOURCES
    "src/*.cpp"
    "src/*.c"
)

# Build as a static library or relocatable object files for decomp diffing
add_library(splatoon_lib STATIC ${SOURCES})

# Target-specific compiler options
if(MSVC)
    target_compile_options(splatoon_lib PRIVATE /W3 /Zc:__cplusplus)
else()
    target_compile_options(splatoon_lib PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
