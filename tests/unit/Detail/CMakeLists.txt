# Enum

add_executable(TomlQt_Tests_Unit_Detail_Enum
        EnumTest.cpp)

target_include_directories(TomlQt_Tests_Unit_Detail_Enum PRIVATE
        ${INCLUDE_DIR}
        ${UNIT_DIR})

target_link_libraries(TomlQt_Tests_Unit_Detail_Enum PRIVATE
        TomlQt::Tests::Unit::_Helpers
        TomlQt::Enum
        TomlQt::Maps
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Test
        fmt::fmt)

add_test(NAME TomlQt_Tests_Unit_Detail_Enum COMMAND TomlQt_Tests_Unit_Detail_Enum)

# QFlags

add_executable(TomlQt_Tests_Unit_Detail_QFlags
        QFlagsTest.cpp)

target_include_directories(TomlQt_Tests_Unit_Detail_QFlags PRIVATE
        ${INCLUDE_DIR}
        ${UNIT_DIR})

target_link_libraries(TomlQt_Tests_Unit_Detail_QFlags PRIVATE
        TomlQt::Tests::Unit::_Helpers
        TomlQt::QFlags
        TomlQt::Maps
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Test
        fmt::fmt)

add_test(NAME TomlQt_Tests_Unit_Detail_QFlags COMMAND TomlQt_Tests_Unit_Detail_QFlags)
