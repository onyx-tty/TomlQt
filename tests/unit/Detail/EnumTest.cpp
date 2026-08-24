// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Enum.h"
#include "TomlQt/Detail/Maps.h"
#include "_Helpers/Helpers.h"

#include <fmt/format.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QObject>
#include <QSizePolicy>
#include <QTest>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

class EnumTest final : public QObject {
        Q_OBJECT

private:
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
                static const std::unordered_map<std::string, Qt::Alignment> map =
                        map::makeAlignment();

                return map;
        }

        [[nodiscard]] static const auto& getSizePolicyMap() {
                static const std::unordered_map<std::string, QSizePolicy> map = map::makeSizePolicy();

                return map;
        }

private slots:
        static void sanityChecks() {
                const auto& example_table = getExampleTable();
                const auto* alignments    = example_table["alignment"].as_table();
                const auto* size_policies = example_table["size_policy"].as_table();

                QVERIFY2(alignments, "example_table[\"alignment\"] must be a valid table");
                QVERIFY2(size_policies, "example_table[\"size_policy\"] must be a valid table");
        }

        static void tryGetEnumFromMap_HandleCorrectQtAlignment() {
                const auto& table         = getExampleTable();
                const auto& node          = table["alignment"];
                const auto& alignment_map = getAlignmentMap();

                for (const auto& [key, value] : alignment_map) {
                        auto result = tryGetEnumFromMap(node[key], alignment_map);
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
                const auto& table           = getExampleTable();
                const auto& node            = table["size_policy"];
                const auto& size_policy_map = getSizePolicyMap();

                for (const auto& [key, value] : size_policy_map) {
                        auto result = tryGetEnumFromMap(node[key], size_policy_map);

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
                const auto& table          = getExampleTable();
                const auto& node           = table["invalid"];
                const auto& alignment_map  = getAlignmentMap();
                const auto  invalid_result = tryGetEnumFromMap(node, getAlignmentMap());

                QVERIFY2(!invalid_result.has_value(), "Result from invalid node must be null");
        }

        static void tryGetEnumFromMapTest_HandleInvalidMap() {
                const auto&                                          table = getExampleTable();
                const auto&                                          node  = table["alignment"];
                const std::unordered_map<std::string, Qt::Alignment> invalid_map = {};

                const auto invalid_result = tryGetEnumFromMap(node["top"], invalid_map);
                QVERIFY2(!invalid_result.has_value(), "Result from invalid map must be null");
        }
};

QTEST_APPLESS_MAIN(EnumTest);
#include "EnumTest.moc"
