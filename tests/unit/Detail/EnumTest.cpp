// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Enum.h"
#include "Helpers.h"
#include "TomlQt/Detail/Maps.h"

#include <cstdlib>
#include <gtest/gtest.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
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

        const auto tester = [&table, &alignment_map](std::string_view key, Qt::Alignment expected) {
                auto result = tryGetEnumFromMap(table[key], alignment_map);
                ASSERT_TRUE(result.has_value());
                ASSERT_EQ(result.value(), expected);
        };

        ASSERT_NO_FATAL_FAILURE(tester("top", Qt::AlignHCenter | Qt::AlignTop));
        ASSERT_NO_FATAL_FAILURE(tester("center", Qt::AlignCenter));
        ASSERT_NO_FATAL_FAILURE(tester("bottom", Qt::AlignHCenter | Qt::AlignBottom));
        ASSERT_NO_FATAL_FAILURE(tester("left", Qt::AlignLeft | Qt::AlignVCenter));
        ASSERT_NO_FATAL_FAILURE(tester("right", Qt::AlignRight | Qt::AlignVCenter));
}

// TODO HandleMixedCaseKey
TEST(TryGetEnumFromMapTest, HandleCorrectQSizePolicy) {
        const auto& table           = getExampleTable()["size_policy"];
        const auto& size_policy_map = getSizePolicyMap();

        const auto tester = [&table, &size_policy_map](std::string_view key, QSizePolicy expected) {
                auto result = tryGetEnumFromMap(table[key], size_policy_map);
                ASSERT_TRUE(result.has_value());
                ASSERT_EQ(result.value(), expected);
        };

        using enum QSizePolicy::Policy;
        ASSERT_NO_FATAL_FAILURE(tester("fixed", {Fixed, Fixed}));
        ASSERT_NO_FATAL_FAILURE(tester("minimum", {Minimum, Minimum}));
        ASSERT_NO_FATAL_FAILURE(tester("maximum", {Maximum, Maximum}));
        ASSERT_NO_FATAL_FAILURE(tester("preferred", {Preferred, Preferred}));
        ASSERT_NO_FATAL_FAILURE(tester("expanding", {Expanding, Expanding}));
        ASSERT_NO_FATAL_FAILURE(tester("minimumexpanding", {MinimumExpanding, MinimumExpanding}));
        ASSERT_NO_FATAL_FAILURE(tester("minimum_expanding", {MinimumExpanding, MinimumExpanding}));
        ASSERT_NO_FATAL_FAILURE(tester("ignored", {Ignored, Ignored}));
}

TEST(TryGetEnumFromMapTest, HandleInvalidNode) {
        const auto& table         = getExampleTable();
        const auto& alignment_map = getAlignmentMap();

        const auto invalid_result = tryGetEnumFromMap(table["invalid"], getAlignmentMap());
        ASSERT_FALSE(invalid_result.has_value());
}

TEST(TryGetEnumFromMapTest, HandleInvalidMap) {
        const auto&                                          table = getExampleTable()["alignment"];
        const std::unordered_map<std::string, Qt::Alignment> invalid_map{};

        const auto invalid_result = tryGetEnumFromMap(table["top"], invalid_map);
        ASSERT_FALSE(invalid_result.has_value());
}
