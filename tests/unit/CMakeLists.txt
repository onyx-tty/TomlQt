set(UNIT_DIR ${CMAKE_CURRENT_LIST_DIR})

add_subdirectory(_Helpers)
add_subdirectory(Detail)

# TomlQt.h + ArrayBounds.h

add_executable(TomlQt_Tests_Unit
        TomlQtTest.cpp)

target_include_directories(TomlQt_Tests_Unit PRIVATE
        ${INCLUDE_DIR}
        ${UNIT_DIR})

target_link_libraries(TomlQt_Tests_Unit PRIVATE
        TomlQt
        tomlplusplus::tomlplusplus
        Qt${QT_VERSION_MAJOR}::Core)

target_link_libraries(TomlQt_Tests_Unit PUBLIC
        Qt${QT_VERSION_MAJOR}::Test)

add_test(NAME TomlQt_Tests_Unit COMMAND TomlQt_Tests_Unit)
