cmake_minimum_required(VERSION 3.14)
project(chylatum VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)
FetchContent_Declare(
    cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(cli11)

# Core library
add_library(chylatum
    chylatum/chylatum.cpp
)
target_include_directories(chylatum PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/chylatum
)

# CLI executable
add_executable(chylatum-cli
    chylatum-cli.cpp
)
target_link_libraries(chylatum-cli PRIVATE
    chylatum
    CLI11::CLI11
)
