// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/QFlags.h"
#include "TomlQt/Detail/Maps.h"
#include "_Helpers/Helpers.h"

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <unordered_map>
#include <QFlags>
#include <QObject>
#include <QTest>
#include <Qt>

using namespace std::string_view_literals;
using namespace tomlqt::detail;

namespace {

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

[[nodiscard]] static const toml::table& example() {
        static const toml::table example = getParsedTable(getRawExampleTable());
        return example;
}

} // namespace

class QFlagsTest final : public QObject {
        Q_OBJECT

private:
        [[nodiscard]] static const auto& alignment() {
                static const auto& _alignment = example()["alignment"];
                return _alignment;
        }

        [[nodiscard]] static const auto& alignmentMap() {
                static const std::unordered_map<std::string, Qt::AlignmentFlag> map =
                        map::makeAlignmentFlag();

                return map;
        }

private slots:
        // TODO HandleMixedCaseKey
        static void tryGetQFlagsFromMap_HandleCorrectQtAlignment() {
                for (const auto& [key, value] : alignmentMap()) {
                        auto result = tryGetQFlagsFromMap(alignment()[key], alignmentMap());

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
                const auto invalid_result = tryGetQFlagsFromMap(alignment()["invalid"],
                                                                alignmentMap());

                QVERIFY2(!invalid_result.has_value(), "Result from invalid node must be null");
        }

        static void tryGetQFlagsFromMapTest_HandleInvalidMap() {
                const std::unordered_map<std::string, Qt::AlignmentFlag> invalid_map = {};

                const auto invalid_result = tryGetQFlagsFromMap(alignment()["top"], invalid_map);
                QVERIFY2(!invalid_result.has_value(), "Result from invalid table must be null");
        }
};

QTEST_APPLESS_MAIN(QFlagsTest);
#include "QFlagsTest.moc"
