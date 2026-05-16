// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "Helpers.h"
#include "TomlQt/Detail/QFlags.h"
#include "TomlQt/Detail/Maps.h"

#include <cstdlib>
#include <gtest/gtest.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QFlags>
#include <QString>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

[[nodiscard]] static consteval std::string_view getRawExampleTable() {
        constexpr std::string_view example = R"(
      [alignment]
      top = ["top", "hcenter"]
      center = ["center"]
      bottom = ["bottom", "hcenter"]
      left = ["vcenter", "left"]
      right = ["vcenter", "right"]
      )"sv;

        return example;
}

[[nodiscard]] static const toml::table& getExampleTable() {
        static const toml::table example = getParsedTable(getRawExampleTable());
        return example;
}

[[nodiscard]] static const auto& getAlignmentFlagMap() {
        static const std::unordered_map<std::string, Qt::AlignmentFlag> map = map::makeAlignmentFlag();

        return map;
}

TEST(TomlQtQFlagsTestHelpersTest, SanityChecks) {
        const auto& example_table = getExampleTable();
        const auto* alignments    = example_table["alignment"].as_table();

        ASSERT_TRUE(alignments);
}

// TODO HandleMixedCaseKey
TEST(TryGetQFlagsFromMapTest, HandleCorrectQtAlignment) {
        const auto& table         = getExampleTable()["alignment"];
        const auto& alignment_map = getAlignmentFlagMap();

        const auto tester = [&table, &alignment_map](std::string_view key, Qt::Alignment expected) {
                auto result = tryGetQFlagsFromMap(table[key], alignment_map);
                ASSERT_TRUE(result.has_value());
                ASSERT_EQ(result.value(), expected);
        };

        ASSERT_NO_FATAL_FAILURE(tester("top", Qt::AlignTop | Qt::AlignHCenter));
        ASSERT_NO_FATAL_FAILURE(tester("center", Qt::AlignCenter));
        ASSERT_NO_FATAL_FAILURE(tester("bottom", Qt::AlignBottom | Qt::AlignHCenter));
        ASSERT_NO_FATAL_FAILURE(tester("left", Qt::AlignVCenter | Qt::AlignLeft));
        ASSERT_NO_FATAL_FAILURE(tester("right", Qt::AlignVCenter | Qt::AlignRight));
}

TEST(TryGetQFlagsFromMapTest, HandleInvalidNode) {
        const auto& table         = getExampleTable();
        const auto& alignment_map = getAlignmentFlagMap();

        const auto invalid_result = tryGetQFlagsFromMap(table["invalid"], alignment_map);
        ASSERT_FALSE(invalid_result.has_value());
}

TEST(TryGetQFlagsFromMapTest, HandleInvalidMap) {
        const auto& table = getExampleTable()["alignment"];
        const std::unordered_map<std::string, Qt::AlignmentFlag> invalid_map{};

        const auto invalid_result = tryGetQFlagsFromMap(table["top"], invalid_map);
        ASSERT_FALSE(invalid_result.has_value());
}
