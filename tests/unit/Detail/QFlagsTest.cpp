// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/QFlags.h"
#include "TomlQt/Detail/Maps.h"
#include "_Helpers/Helpers.h"

#include <fmt/format.h>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QFlags>
#include <QObject>
#include <QString>
#include <QTest>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

class QFlagsTest final : public QObject {
        Q_OBJECT

private:
        [[nodiscard]] static consteval std::string_view getRawExampleTable() {
                constexpr std::string_view example = R"(
      [alignment]
      top = ["top"]
      center = ["center"]
      hcenter = ["hcenter"]
      vcenter = ["vcenter"]
      bottom = ["bottom"]
      left = ["left"]
      right = ["right"]
      )"sv;

                return example;
        }

        [[nodiscard]] static const toml::table& getExampleTable() {
                static const toml::table example = getParsedTable(getRawExampleTable());
                return example;
        }

        [[nodiscard]] static const auto& getAlignmentFlagMap() {
                static const std::unordered_map<std::string, Qt::AlignmentFlag> map =
                        map::makeAlignmentFlag();

                return map;
        }

private slots:
        static void sanityChecks() {
                const auto& example_table = getExampleTable();
                const auto* alignments    = example_table["alignment"].as_table();

                QVERIFY2(alignments, "example_table[\"alignment\"] must be a valid table");
        }

        // TODO HandleMixedCaseKey
        static void tryGetQFlagsFromMap_HandleCorrectQtAlignment() {
                const auto& table         = getExampleTable();
                const auto& node          = table["alignment"];
                const auto& alignment_map = getAlignmentFlagMap();

                for (const auto& [key, value] : alignment_map) {
                        auto result = tryGetQFlagsFromMap(node[key], alignment_map);
                        QVERIFY2(result.has_value(),
                                 fmt::format("Key {} must be found in table", key).c_str());
                        QVERIFY2(result.value() == value,
                                 fmt::format("Key {} must correspond to {}, but it's {}", key,
                                             static_cast<int>(value),
                                             static_cast<int>(result.value()))
                                         .c_str());
                }
        }

        static void tryGetQFlagsFromMapTest_HandleInvalidNode() {
                const auto& table         = getExampleTable();
                const auto& node          = table["invalid"];
                const auto& alignment_map = getAlignmentFlagMap();

                const auto invalid_result = tryGetQFlagsFromMap(node, alignment_map);
                QVERIFY2(!invalid_result.has_value(), "Result from invalid node must be null");
        }

        static void tryGetQFlagsFromMapTest_HandleInvalidMap() {
                const auto&                                              table = getExampleTable();
                const auto&                                              node  = table["alignment"];
                const std::unordered_map<std::string, Qt::AlignmentFlag> invalid_map = {};

                const auto invalid_result = tryGetQFlagsFromMap(node["top"], invalid_map);
                QVERIFY2(!invalid_result.has_value(), "Result from invalid table must be null");
        }
};

QTEST_APPLESS_MAIN(QFlagsTest);
#include "QFlagsTest.moc"
