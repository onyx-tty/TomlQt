cmake_minimum_required(VERSION 3.14)
project(TomlQt VERSION 0.6.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)

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
option(CPPUTILS_ENABLE_QT "Enable Qt in CppUtils" ON)

find_package(tomlplusplus REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(NOT TARGET CppUtils)
        find_package(CppUtils QUIET)
endif()

if(NOT TARGET CppUtils)
        add_subdirectory(external/CppUtils)
endif()

# Store and propagate INCLUDE_DIR and SRC_DIR for use in target_include_directories
set(ROOT_DIR
        ${CMAKE_CURRENT_LIST_DIR})

set(INCLUDE_DIR
        ${ROOT_DIR}/include)

add_subdirectory(src)

if(BUILD_TESTS)
        find_package(GTest REQUIRED)
        enable_testing()
        add_subdirectory(tests)
endif()
