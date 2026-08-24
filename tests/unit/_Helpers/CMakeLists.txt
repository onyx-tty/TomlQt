add_library(TomlQt_Tests_Unit__Helpers
        Helpers.cpp)
add_library(TomlQt::Tests::Unit::_Helpers ALIAS TomlQt_Tests_Unit__Helpers)

target_include_directories(TomlQt_Tests_Unit__Helpers PRIVATE
        ${INCLUDE_DIR}
        ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(TomlQt_Tests_Unit__Helpers PUBLIC
        tomlplusplus::tomlplusplus)
