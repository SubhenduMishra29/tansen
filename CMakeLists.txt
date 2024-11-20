cmake_minimum_required(VERSION 3.10)

# Project Name
project(Tansen VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Directories
include_directories(include)

# Sources
set(SOURCES
    src/TaalManager.cpp
    src/MIDIHandler.cpp
    src/Tempo.cpp
    src/CommandParser.cpp
    src/main.cpp
)

# Executable
add_executable(Tansen ${SOURCES})

# Dependencies
find_package(nlohmann_json REQUIRED) # For handling JSON

# Link libraries (e.g., JSON library)
target_link_libraries(Tansen PRIVATE nlohmann_json::nlohmann_json)

# Output Directory
set_target_properties(Tansen PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install Configuration
install(TARGETS Tansen DESTINATION bin)

# Additional Notes
message(STATUS "Tansen project successfully configured!")
