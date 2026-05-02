# TomlQt::Enum
add_library(TomlQt_Enum INTERFACE)
add_library(TomlQt::Enum ALIAS TomlQt_Enum)

target_include_directories(TomlQt_Enum INTERFACE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(TomlQt_Enum INTERFACE
        CppUtils::String
        CppUtils::Conversion
        CppUtils::Log::Qt)

target_link_libraries(TomlQt_Enum INTERFACE
        Qt${QT_VERSION_MAJOR}::Widgets
        tomlplusplus::tomlplusplus)

# TomlQt::QFlags
add_library(TomlQt_QFlags INTERFACE)
add_library(TomlQt::QFlags ALIAS TomlQt_QFlags)

target_include_directories(TomlQt_QFlags INTERFACE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(TomlQt_QFlags INTERFACE
        CppUtils::String
        CppUtils::Conversion
        CppUtils::Log::Qt)

target_link_libraries(TomlQt_QFlags INTERFACE
        Qt${QT_VERSION_MAJOR}::Core
        tomlplusplus::tomlplusplus)

# TomlQt::Maps
add_library(TomlQt_Maps
        Maps.cpp)
add_library(TomlQt::Maps ALIAS TomlQt_Maps)

target_include_directories(TomlQt_Maps PRIVATE
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

target_link_libraries(TomlQt_Maps PUBLIC
        Qt${QT_VERSION_MAJOR}::Widgets)
