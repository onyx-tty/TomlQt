// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/TomlQt.h"

#include <gtest/gtest.h>
#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

using namespace std::string_view_literals;
using namespace tomlqt;

[[nodiscard]] static consteval std::string_view getRawExampleTable() {
        constexpr std::string_view example = R"(
      [values]
      name = "TomlQt"
      year = 2026
      qsize = [100, 50]
      small_qsize = [100]
      large_qsize = [100, 50, 12, 300, 10]
      )"sv;

        return example;
}

[[nodiscard]] static toml::table getParsedExampleTable(std::string_view example) {
        try {
                toml::table parsed = toml::parse(example);
                return parsed;
        } catch (const toml::parse_error& err) {
                std::cerr << "Parsing failed:\n" << err << "\n";
                exit(-1);
        }
}

[[nodiscard]] static const toml::table& getExampleTable() {
        static const toml::table example = getParsedExampleTable(getRawExampleTable());
        return example;
}

TEST(TomlQtTestHelpersTest, SanityChecks) {
        const auto& example_table = getExampleTable();
        const auto* table         = example_table["values"].as_table();

        ASSERT_TRUE(table);
}

namespace {
const auto table = *getExampleTable()["values"].as_table();
}

TEST(TomlQtAsArrayWithBoundsTest, HandlesNoBoundChecking) {
        const auto node = table["qsize"];
        ASSERT_TRUE(asArrayWithBounds(node, {}));
}

TEST(TomlQtAsArrayWithBoundsTest, HandlesNull) {
        const auto arr  = toml::array();
        const auto node = toml::node_view<const toml::node>(arr);
        ASSERT_FALSE(asArrayWithBounds(node, {.min_size = 10}));
}

TEST(TomlQtAsArrayWithBoundsTest, HandlesMinSizeFail) {
        const auto node = table["qsize"];
        ASSERT_FALSE(asArrayWithBounds(node, {.min_size = 10}));
}

TEST(TomlQtAsArrayWithBoundsTest, HandlesMaxSizeFail) {
        const auto node = table["qsize"];
        ASSERT_FALSE(asArrayWithBounds(node, {.max_size = 0}));
}

TEST(TomlQtAsArrayWithBoundsTest, HandlesWrongType) {
        const auto arr  = toml::value<bool>();
        const auto node = toml::node_view<const toml::node>(arr);
        ASSERT_FALSE(asArrayWithBounds(node, {.min_size = 10}));
}

TEST(TomlQtValueQSizeTest, HandlesCorrectNode) {
        const std::optional<QSize> qsize_res = value<QSize>(table["qsize"]);
        ASSERT_TRUE(qsize_res.has_value());

        const QSize size = {100, 50};
        EXPECT_EQ(qsize_res.value(), size);
}

TEST(TomlQtValueQSizeTest, HandlesEmptyNode) {
        const std::optional<QSize> qsize_res = value<QSize>(table["does_not_exist"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TomlQtValueQSizeTest, HandlesWrongType) {
        const std::optional<QSize> qsize_res = value<QSize>(table["year"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TomlQtValueQSizeTest, HandlesInsufficientIndex) {
        const std::optional<QSize> qsize_res = value<QSize>(table["small_qsize"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TomlQtValueQSizeTest, HandlesLargerArrays) {
        const std::optional<QSize> qsize_res = value<QSize>(table["large_qsize"]);
        ASSERT_TRUE(qsize_res.has_value());

        const QSize size = {100, 50};
        EXPECT_EQ(qsize_res.value(), size);
}

TEST(TomlQtValueQStringTest, HandlesCorrectNode) {
        const std::optional<QString> str_res = value<QString>(table["name"]);
        ASSERT_TRUE(str_res.has_value());

        const QString name = "TomlQt";
        EXPECT_EQ(str_res.value(), name);
}

TEST(TomlQtValueQStringTest, HandlesEmptyNode) {
        const std::optional<QString> str_res = value<QString>(table["does_not_exist"]);
        EXPECT_FALSE(str_res.has_value());
}

TEST(TomlQtValueQStringTest, HandlesWrongType) {
        const std::optional<QString> str_res = value<QString>(table["year"]);
        EXPECT_FALSE(str_res.has_value());
}
