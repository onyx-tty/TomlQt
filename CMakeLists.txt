cmake_minimum_required(VERSION 3.14)
project(TomlQt VERSION 0.6.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)

# Install and use CPM to resolve conflicting and duplicated CppUtils
file(
        DOWNLOAD
        https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.1/CPM.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
        EXPECTED_HASH SHA256=f3a6dcc6a04ce9e7f51a127307fa4f699fb2bade357a8eb4c5b45df76e1dc6a5
)
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)

if(ENABLE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY
                clang-tidy;
                -header-filter=.;)
endif()

# Export compile_commands.json to ensure language servers work properly
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build shared by default
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)
option(BUILD_TESTS "Build tests" ON)
option(LOG_DEBUG "Print debug-level logs" OFF)
set(CPPUTILS_ENABLE_QT ON)

find_package(tomlplusplus REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

CPMAddPackage("gh:onyx-tty/CppUtils#926ee69")

set(ROOT_DIR
        ${CMAKE_CURRENT_LIST_DIR})

# Store and propagate INCLUDE_DIR for use in target_include_directories
set(INCLUDE_DIR
        ${ROOT_DIR}/include)

add_subdirectory(src)

if(BUILD_TESTS)
        find_package(GTest REQUIRED)
        enable_testing()
        add_subdirectory(tests)
endif()
