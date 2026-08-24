// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/TomlQt.h"
#include "TomlQt/ArrayBounds.h"

#include <fmt/format.h>
#include <iostream>
#include <optional>
#include <string_view>
#include <toml++/toml.hpp>
#include <QObject>
#include <QSize>
#include <QString>
#include <QTest>

using namespace std::string_view_literals;
using namespace tomlqt;

namespace {

[[nodiscard]] consteval std::string_view getRawExampleTable() {
        constexpr std::string_view example = R"(
      name = "TomlQt"
      year = 2026
      qsize = [100, 50]
      small_qsize = [100]
      large_qsize = [100, 50, 12, 300, 10]
      )"sv;

        return example;
}

[[nodiscard]] toml::table getParsedExampleTable(std::string_view example) {
        try {
                toml::table parsed = toml::parse(example);
                return parsed;
        } catch (const toml::parse_error& err) {
                std::cerr << "Parsing failed:\n" << err << "\n";
                exit(-1);
        }
}

} // namespace

class TomlQtTest : public QObject {
        Q_OBJECT

private:
        using enum ArrayBounds::validation_result;

        static const toml::table& example() {
                static const auto _example = getParsedExampleTable(getRawExampleTable());
                return _example;
        }

private slots:
        static void ArrayBounds_validate_HandlesAllBoundsNull() {
                const toml::array* arr = example()["small_qsize"].as_array();

                QVERIFY2(ArrayBounds().validate(arr) == success,
                         "Must return success for null bounds with non-null pointer to non-empty array");
                QVERIFY2(ArrayBounds().validate(*arr) == success,
                         "Must return success bounds with non-empty array");
        }

        static void ArrayBounds_validate_HandlesMinSizeFail() {
                const toml::array* arr    = example()["small_qsize"].as_array();
                ArrayBounds        bounds = {.min_size = 2};

                QVERIFY2(bounds.validate(arr) == min_size_fail,
                         "Must return min_size_fail for failed min_size bound check");
                QVERIFY2(bounds.validate(*arr) == min_size_fail,
                         "Must return min_size_fail for failed min_size bound check");
        }

        static void ArrayBounds_validate_HandlesMaxSizeFail() {
                const toml::array* arr    = example()["large_qsize"].as_array();
                ArrayBounds        bounds = {.max_size = 2};

                QVERIFY2(bounds.validate(arr) == max_size_fail,
                         "Must return max_size_fail for failed max_size bound check");
                QVERIFY2(bounds.validate(*arr) == max_size_fail,
                         "Must return max_size_fail for failed max_size bound check");
        }

        static void ArrayBounds_validate_HandlesNullArray() {
                QVERIFY2(ArrayBounds().validate(nullptr) == null_ptr,
                         "Must return null_ptr for null array");
        }

        static void asArrayWithBounds_HandlesNull() {
                const auto arr  = toml::array();
                const auto node = toml::node_view<const toml::node>(arr);

                QVERIFY2(!asArrayWithBounds(node, {.min_size = 10}),
                         "Must return nullptr for empty array");
                QVERIFY2(!asArrayWithBounds({}, {.min_size = 10}),
                         "Must return nullptr for null node");
        }

        static void asArrayWithBounds_HandlesMinSizeFail() {
                const auto node = example()["qsize"];

                QVERIFY2(!asArrayWithBounds(node, {.min_size = 10}),
                         "Must return nullptr for failed min_size bound check");
        }

        static void asArrayWithBounds_HandlesMaxSizeFail() {
                const auto node = example()["qsize"];

                QVERIFY2(!asArrayWithBounds(node, {.max_size = 0}),
                         "Must return nullptr for failed max_size bound check");
        }

        static void asArrayWithBounds_HandlesWrongType() {
                const auto arr  = toml::value<bool>();
                const auto node = toml::node_view<const toml::node>(arr);

                QVERIFY2(!asArrayWithBounds(node, {}), "Must return nullptr for the wrong type");
        }

        static void valueQSize_HandlesCorrectNode() {
                const auto node = example()["qsize"];

                const std::optional<QSize> qsize_res = value<QSize>(node);
                QVERIFY2(qsize_res.has_value(), "example_table[qsize] must be valid");

                const QSize size = {100, 50};
                QVERIFY2(qsize_res.value() == size,
                         "Must be able to interpret an array of 2 numbers as QSize");
        }

        static void valueQSize_HandlesEmptyNode() {
                const auto& node = example()["does_not_exist"];

                const std::optional<QSize> qsize_res = value<QSize>(node);
                QVERIFY2(!qsize_res.has_value(), "Must return nullopt for empty nodes");
        }

        static void valueQSize_HandlesWrongType() {
                const auto& node = example()["year"];

                const std::optional<QSize> qsize_res = value<QSize>(node);
                QVERIFY2(!qsize_res.has_value(), "Must return nullopt for wrong types");
        }

        static void valueQSize_HandlesInsufficientIndex() {
                const auto& node = example()["small_qsize"];

                const std::optional<QSize> qsize_res = value<QSize>(node);
                QVERIFY2(!qsize_res.has_value(),
                         "Must return nullopt for arrays with less than 2 elements");
        }

        static void valueQSize_HandlesLargerArrays() {
                const auto& node = example()["large_qsize"];

                const std::optional<QSize> qsize_res = value<QSize>(node);
                QVERIFY2(qsize_res.has_value(), "Must be valid even with more than 2 elements");

                const QSize size = {100, 50};
                QVERIFY2(qsize_res.value() == size,
                         "Must return QSize with expected width and height");
        }

        static void valueQString_HandlesCorrectNode() {
                const auto& node = example()["name"];

                const std::optional<QString> str_res = value<QString>(node);
                QVERIFY2(str_res.has_value(), "Must return QString from valid node");

                const QString name = "TomlQt";
                QVERIFY2(str_res.value() == name, "Must return expected QString");
        }

        static void valueQString_HandlesEmptyNode() {
                const auto& node = example()["does_not_exist"];

                const std::optional<QString> str_res = value<QString>(node);
                QVERIFY2(!str_res.has_value(), "Must return nullopt from null node");
        }

        static void valueQString_HandlesWrongType() {
                const auto& node = example()["year"];

                const std::optional<QString> str_res = value<QString>(node);
                QVERIFY2(!str_res.has_value(), "Must return nullopt from wrong node type");
        }
};

QTEST_APPLESS_MAIN(TomlQtTest);
#include "TomlQtTest.moc"
