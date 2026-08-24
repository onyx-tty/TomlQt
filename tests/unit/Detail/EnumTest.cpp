// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Enum.h"
#include "Helpers.h"
#include "TomlQt/Detail/Maps.h"

#include <gtest/gtest.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QSizePolicy>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

[[nodiscard]] static consteval std::string_view getRawExampleTable() {
        constexpr std::string_view example = R"(
      [alignment]
      top = "top"
      center = "center"
      bottom = "bottom"
      left = "left"
      right = "right"

      [size_policy]
      fixed = "fixed"
      minimum = "minimum"
      maximum = "maximum"
      preferred = "preferred"
      expanding = "expanding"
      minimumexpanding = "minimumexpanding"
      minimum_expanding = "minimum_expanding"
      ignored = "ignored"
      )"sv;

        return example;
}

[[nodiscard]] static const toml::table& getExampleTable() {
        static const toml::table example = getParsedTable(getRawExampleTable());
        return example;
}

[[nodiscard]] static const auto& getAlignmentMap() {
        static const std::unordered_map<std::string, Qt::Alignment> map = map::makeAlignment();

        return map;
}

[[nodiscard]] static const auto& getSizePolicyMap() {
        static const std::unordered_map<std::string, QSizePolicy> map = map::makeSizePolicy();

        return map;
}

TEST(TomlQtEnumTestHelpersTest, SanityChecks) {
        const auto& example_table = getExampleTable();
        const auto* alignments    = example_table["alignment"].as_table();
        const auto* size_policies = example_table["size_policy"].as_table();

        ASSERT_TRUE(alignments);
        ASSERT_TRUE(size_policies);
}

TEST(TryGetEnumFromMapTest, HandleCorrectQtAlignment) {
        const auto& table         = getExampleTable()["alignment"];
        const auto& alignment_map = getAlignmentMap();

        for (auto [key, value] : alignment_map) {
                auto result = tryGetEnumFromMap(table[key], alignment_map);
                EXPECT_TRUE(result.has_value());
                ASSERT_EQ(result.value(), value);
        }
}

// TODO HandleMixedCaseKey
TEST(TryGetEnumFromMapTest, HandleCorrectQSizePolicy) {
        using enum QSizePolicy::Policy;

        const auto& table           = getExampleTable()["size_policy"];
        const auto& size_policy_map = getSizePolicyMap();

        for (const auto& [key, value] : size_policy_map) {
                auto result = tryGetEnumFromMap(table[key], size_policy_map);
                EXPECT_TRUE(result.has_value());
                ASSERT_EQ(result.value(), value);
        }
}

TEST(TryGetEnumFromMapTest, HandleInvalidNode) {
        const auto& table         = getExampleTable();
        const auto& alignment_map = getAlignmentMap();

        const auto invalid_result = tryGetEnumFromMap(table["invalid"], getAlignmentMap());
        ASSERT_FALSE(invalid_result.has_value());
}

TEST(TryGetEnumFromMapTest, HandleInvalidMap) {
        const auto&                                          table = getExampleTable()["alignment"];
        const std::unordered_map<std::string, Qt::Alignment> invalid_map = {};

        const auto invalid_result = tryGetEnumFromMap(table["top"], invalid_map);
        ASSERT_FALSE(invalid_result.has_value());
}
