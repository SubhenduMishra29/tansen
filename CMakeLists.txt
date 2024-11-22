cmake_minimum_required(VERSION 3.10)

# Project Name
project(Tansen VERSION 1.0 LANGUAGES CXX)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Directories
include_directories(include)

# Find Flex and Bison
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

# Define Flex and Bison files in cli directory
set(FLEX_FILE ${CMAKE_SOURCE_DIR}/cli/command_lexer.l)
set(BISON_FILE ${CMAKE_SOURCE_DIR}/cli/command_parser.y)

# Generate sources for Flex and Bison
FLEX_TARGET(CommandLexer ${FLEX_FILE} ${CMAKE_BINARY_DIR}/cli/command_lexer.cpp)
BISON_TARGET(CommandParser ${BISON_FILE} ${CMAKE_BINARY_DIR}/cli/command_parser.cpp
    DEFINES_FILE ${CMAKE_BINARY_DIR}/cli/command_parser.h)
ADD_FLEX_BISON_DEPENDENCY(CommandLexer CommandParser)

# Include Flex/Bison output in sources
set(CLI_SOURCES
    ${FLEX_CommandLexer_OUTPUTS}
    ${BISON_CommandParser_OUTPUTS}
)

# Sources
set(SOURCES
    src/TaalManager.cpp
    src/MIDIHandler.cpp
    src/Tempo.cpp
    ${CLI_SOURCES}
    src/main.cpp
)

# Executable
add_executable(Tansen ${SOURCES})

# Dependencies
find_package(nlohmann_json REQUIRED) # For handling JSON

# Link libraries (e.g., JSON library)
target_link_libraries(Tansen PRIVATE nlohmann_json::nlohmann_json)

# Include the directory for generated parser headers
target_include_directories(Tansen PRIVATE ${CMAKE_BINARY_DIR}/cli)

# Output Directory
set_target_properties(Tansen PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Install Configuration
install(TARGETS Tansen DESTINATION bin)

# Additional Notes
message(STATUS "Tansen project successfully configured!")
