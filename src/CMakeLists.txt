add_subdirectory(Detail)

add_library(TomlQt
        TomlQt.cpp)

target_include_directories(TomlQt PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(TomlQt PRIVATE
        TomlQt::Enum
        TomlQt::QFlags
        TomlQt::Maps
        CppUtils::Log::Qt)

target_link_libraries(TomlQt PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)
