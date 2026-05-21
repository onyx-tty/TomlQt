cmake_minimum_required(VERSION 3.21)
project(TomlQt VERSION 0.7.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(QT_VERSION_MAJOR 6)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

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
option(LOG_DEBUG "Print debug-level logs" OFF)
set(CPPUTILS_ENABLE_QT ON)

find_package(tomlplusplus REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

CPMAddPackage("gh:onyx-tty/CppUtils#53cccec")

set(ROOT_DIR
        ${CMAKE_CURRENT_LIST_DIR})

# Store and propagate INCLUDE_DIR for use in target_include_directories
set(INCLUDE_DIR
        ${ROOT_DIR}/include)

add_subdirectory(src)

if(PROJECT_IS_TOP_LEVEL)
        find_package(GTest REQUIRED)
        enable_testing()
        add_subdirectory(tests)
endif()

install(# Install compiled artifacts to correct destinations, register all targets
        # under the name TomlQt_export
        TARGETS TomlQt
                TomlQt_Enum TomlQt_Maps TomlQt_QFlags # details (temporary)
        EXPORT TomlQt_export
        ARCHIVE  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        LIBRARY  DESTINATION ${CMAKE_INSTALL_LIBDIR}
        RUNTIME  DESTINATION ${CMAKE_INSTALL_BINDIR}
        INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Write resulting TomlQt_export to TomlQt_export.cmake
install(EXPORT TomlQt_export
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/TomlQt)

# Process the Config.cmake.in template, write TomlQtConfig.cmake
# to the build directory
configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/Config.cmake.in
        "${CMAKE_CURRENT_BINARY_DIR}/TomlQtConfig.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_DATADIR}/cmake/TomlQt")

# Generate TomlQtConfigVersion.cmake to ensure version
# requirements are satisfied by the consumer
write_basic_package_version_file(
        TomlQtConfigVersion.cmake
        VERSION ${PACKAGE_VERSION}
        COMPATIBILITY AnyNewerVersion)

# Copy both generated cmake files into the install tree
install(
        FILES ${CMAKE_CURRENT_BINARY_DIR}/TomlQtConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/TomlQtConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_DATADIR}/cmake/TomlQt)
