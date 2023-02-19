cmake_minimum_required(VERSION 3.10)

project(x)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find WasmEdge library
find_package(WasmEdge PATHS /path/to/wasmedge)

# Add executable target
add_executable(${PROJECT_NAME} src/main.cpp)

# Link WasmEdge library
target_link_libraries(${PROJECT_NAME} -lwasmedge)

