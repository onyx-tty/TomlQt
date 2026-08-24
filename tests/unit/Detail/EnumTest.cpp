// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Enum.h"
#include "TomlQt/Detail/Maps.h"
#include "_Helpers/Helpers.h"

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QObject>
#include <QSizePolicy>
#include <QTest>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

namespace {

[[nodiscard]] consteval std::string_view getRawExampleTable() {
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

[[nodiscard]] const toml::table& example() {
        static const toml::table example = getParsedTable(getRawExampleTable());
        return example;
}

} // namespace

class EnumTest final : public QObject {
        Q_OBJECT

private:
        [[nodiscard]] static const auto& alignment() {
                static const auto& _alignment = example()["alignment"];
                return _alignment;
        }

        [[nodiscard]] static const auto& sizePolicy() {
                static const auto& _size_policy = example()["size_policy"];
                return _size_policy;
        }

        [[nodiscard]] static const auto& alignmentMap() {
                static const std::unordered_map<std::string, Qt::Alignment> map =
                        map::makeAlignment();

                return map;
        }

        [[nodiscard]] static const auto& sizePolicyMap() {
                static const std::unordered_map<std::string, QSizePolicy> map = map::makeSizePolicy();

                return map;
        }

private slots:
        static void tryGetEnumFromMap_HandleCorrectQtAlignment() {
                for (const auto& [key, value] : alignmentMap()) {
                        auto result = tryGetEnumFromMap(alignment()[key], alignmentMap());
                        QVERIFY2(result.has_value(),
                                 fmt::format("Key {} must be found in table", key).c_str());

                        QVERIFY2(result.value() == value,
                                 fmt::format("Key {} must correspond to {}, but it's {}", key,
                                             static_cast<int>(value),
                                             static_cast<int>(result.value()))
                                         .c_str());
                }
        }

        // TODO HandleMixedCaseKey
        static void tryGetEnumFromMapTest_HandleCorrectQSizePolicy() {
                for (const auto& [key, value] : sizePolicyMap()) {
                        auto result = tryGetEnumFromMap(sizePolicy()[key], sizePolicyMap());

                        QVERIFY2(result.has_value(), "Enum not found");
                        QVERIFY2(result.value() == value,
                                 fmt::format("Key {} must correspond to {}, but it's {}", key,
                                             value.horizontalPolicy() + value.verticalPolicy(),
                                             result.value().horizontalPolicy()
                                                     + result.value().verticalPolicy())
                                         .c_str());
                }
        }

        static void tryGetEnumFromMap_HandleInvalidNode() {
                const auto invalid_result = tryGetEnumFromMap(alignment()["invalid"],
                                                              alignmentMap());

                QVERIFY2(!invalid_result.has_value(), "Result from invalid node must be null");
        }

        static void tryGetEnumFromMapTest_HandleInvalidMap() {
                const std::unordered_map<std::string, Qt::Alignment> invalid_map = {};

                const auto invalid_result = tryGetEnumFromMap(alignment()["top"], invalid_map);
                QVERIFY2(!invalid_result.has_value(), "Result from invalid map must be null");
        }
};

QTEST_APPLESS_MAIN(EnumTest);
#include "EnumTest.moc"
