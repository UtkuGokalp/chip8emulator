cmake_minimum_required(VERSION 3.16)

project(chip8emulator LANGUAGES CXX)

# Use C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect all source files
file(GLOB CHIP8_SOURCES
    src/*.cpp
)

# Create executable
add_executable(chip8emulator ${CHIP8_SOURCES})

# Include headers
target_include_directories(chip8emulator PRIVATE include)

# Link system libraries
target_link_libraries(chip8emulator
    PRIVATE
        X11
        GL
        pthread
        png
        pulse
        pulse-simple
)

# Output binary to bin/
set_target_properties(chip8emulator PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)
