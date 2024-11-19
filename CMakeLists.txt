# CMake configuration for MIDIProject

cmake_minimum_required(VERSION 3.15)
project(MIDIProject VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Source and Header files
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)

set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/midi_generator.cpp
    ${SRC_DIR}/taal.cpp
    ${SRC_DIR}/rhythm_pattern.cpp
)

set(HEADERS
    ${INCLUDE_DIR}/midi_generator.h
    ${INCLUDE_DIR}/taal.h
    ${INCLUDE_DIR}/rhythm_pattern.h
)

# Include directories
include_directories(${INCLUDE_DIR})

# Output executable
add_executable(MIDIProject ${SOURCES} ${HEADERS})

# Find RtMidi library
find_package(RtMidi REQUIRED)
if (RtMidi_FOUND)
    message(STATUS "RtMidi found: ${RtMidi_LIBRARIES}")
    target_include_directories(MIDIProject PRIVATE ${RtMidi_INCLUDE_DIRS})
    target_link_libraries(MIDIProject PRIVATE ${RtMidi_LIBRARIES})
else()
    message(FATAL_ERROR "RtMidi library not found!")
endif()

# Add JSON library (for loading tal data)
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)
target_link_libraries(MIDIProject PRIVATE nlohmann_json::nlohmann_json)

# Enable warnings
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(MIDIProject PRIVATE -Wall -Wextra -pedantic)
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    target_compile_options(MIDIProject PRIVATE /W4)
endif()
