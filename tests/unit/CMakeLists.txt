add_executable(TomlQt_UnitTests
        TomlQtTest.cpp
        Helpers.cpp
        Detail/EnumTest.cpp
        Detail/QFlagsTest.cpp)

target_include_directories(TomlQt_UnitTests PRIVATE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(TomlQt_UnitTests PRIVATE
        TomlQt
        TomlQt::Enum
        TomlQt::QFlags
        TomlQt::Maps
        CppUtils::Conversion
        CppUtils::String)

target_link_libraries(TomlQt_UnitTests PUBLIC
        GTest::gtest
        GTest::gtest_main)

add_test(NAME TomlQt_UnitTests COMMAND TomlQt_UnitTests)
