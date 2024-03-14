cmake_minimum_required(VERSION 3.20)
# Set project name (editable)
project(PipeRecorder)
set(TEST_NAME ${PROJECT_NAME}Test)

# Set C++ standard to C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# Set compiler flags for all platforms
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-Wall -Wextra)
elseif(MSVC)
    add_compile_options(/W4)
endif()

# Set source and header directories
set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
# Add include directories
include_directories(${INCLUDE_DIR})

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build)

# Generate executable
add_library(${PROJECT_NAME} ${SOURCE_DIR}/Recorder.cpp)
add_executable(${TEST_NAME} ${SOURCE_DIR}/main.cpp)
target_link_libraries(${TEST_NAME} ${PROJECT_NAME})

# Set clangd compilation database
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)