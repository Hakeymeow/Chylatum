cmake_minimum_required(VERSION 3.14)
project(chylatum VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(FetchContent)

# Core library
add_library(chylatum
    chylatum/chylatum.cpp
)
target_include_directories(chylatum PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/chylatum
)

# CLI executable
FetchContent_Declare(cli11
    GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
    GIT_TAG v2.4.2
)
FetchContent_MakeAvailable(cli11)

add_executable(chylatum-cli
    chylatum-cli.cpp
)
target_link_libraries(chylatum-cli PRIVATE
    chylatum
    CLI11::CLI11
)

# TUI executable
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
  GIT_TAG v6.1.9
)
FetchContent_MakeAvailable(ftxui)
add_executable(chylatum-tui
    chylatum-tui.cpp
)
target_link_libraries(chylatum-tui PRIVATE
    chylatum
    ftxui::screen
    ftxui::dom
    ftxui::component
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
