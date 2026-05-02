// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt.h"

#include <gtest/gtest.h>
#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <QSize>
#include <QString>

using namespace std::string_view_literals;

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
static const auto table = *getExampleTable()["values"].as_table();
}

TEST(TryGetQSizeTest, HandlesCorrectNode) {
        const std::optional<QSize> qsize_res = tomlqt::tryGetQSize(table["qsize"]);
        ASSERT_TRUE(qsize_res.has_value());

        const QSize size = {100, 50};
        EXPECT_EQ(qsize_res.value(), size);
}

TEST(TryGetQSizeTest, HandlesEmptyNode) {
        const std::optional<QSize> qsize_res = tomlqt::tryGetQSize(table["does_not_exist"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TryGetQSizeTest, HandlesWrongType) {
        const std::optional<QSize> qsize_res = tomlqt::tryGetQSize(table["year"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TryGetQSizeTest, HandlesInsufficientIndex) {
        const std::optional<QSize> qsize_res = tomlqt::tryGetQSize(table["small_qsize"]);
        EXPECT_FALSE(qsize_res.has_value());
}

TEST(TryGetQSizeTest, HandlesLargerArrays) {
        const std::optional<QSize> qsize_res = tomlqt::tryGetQSize(table["large_qsize"]);
        ASSERT_TRUE(qsize_res.has_value());

        const QSize size = {100, 50};
        EXPECT_EQ(qsize_res.value(), size);
}

TEST(TryGetQStringTest, HandlesCorrectNode) {
        const std::optional<QString> str_res = tomlqt::tryGetQString(table["name"]);
        ASSERT_TRUE(str_res.has_value());

        const QString name = "TomlQt";
        EXPECT_EQ(str_res.value(), name);
}

TEST(TryGetQStringTest, HandlesEmptyNode) {
        const std::optional<QString> str_res = tomlqt::tryGetQString(table["does_not_exist"]);
        EXPECT_FALSE(str_res.has_value());
}

TEST(TryGetQStringTest, HandlesWrongType) {
        const std::optional<QString> str_res = tomlqt::tryGetQString(table["year"]);
        EXPECT_FALSE(str_res.has_value());
}
