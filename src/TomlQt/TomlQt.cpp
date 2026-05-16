// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/TomlQt.h"
#include "CppUtils/Log/QtLog.h"
#include "TomlQt/ArrayBounds.h"
#include "TomlQt/Detail/Enum.h"
#include "TomlQt/Detail/Maps.h"
#include "TomlQt/Detail/QFlags.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <QFlag>
#include <QSize>
#include <QString>

std::optional<QSize> tomlqt::tryGetQSize(toml::node_view<const toml::node> node) {
        using result                    = tomlqt::ArrayBounds::validation_result;
        const auto       arr_conditions = tomlqt::ArrayBounds{.min_size = 2};

        const auto* arr = node.as_array();
        if (!arr) { return std::nullopt; }

        auto res = arr_conditions.validate(*arr);
        if (res == result::min_size_fail) {
                QWARNING() << QString("QSize requires 2 numbers for construction! Numbers provided: %1")
                                      .arg(QString::number(arr->size()));
                return std::nullopt;
        }

        auto width  = (*arr)[0].value<int64_t>();
        auto height = (*arr)[1].value<int64_t>();

        if (!width || !height) { return std::nullopt; }

        // TODO Handle narrowing conversion
        return QSize(width.value(), height.value());
}

std::optional<QString> tomlqt::tryGetQString(toml::node_view<const toml::node> node) {
        if (auto str = node.value<std::string>()) { return QString::fromStdString(str.value()); }

        return std::nullopt;
}

std::optional<Qt::Alignment> tomlqt::tryGetQtAlignment(toml::node_view<const toml::node> node) {
        if (node.is_string()) {
                const static std::unordered_map<std::string, Qt::Alignment> map =
                        tomlqt::detail::map::makeAlignment();

                return tomlqt::detail::tryGetEnumFromMap(node, map);
        }
        if (node.is_array()) {
                const static std::unordered_map<std::string, Qt::AlignmentFlag> map =
                        tomlqt::detail::map::makeAlignmentFlag();
                return tomlqt::detail::tryGetQFlagsFromMap(node, map);
        }

        return std::nullopt;
}

std::optional<QSizePolicy> tomlqt::tryGetQSizePolicy(toml::node_view<const toml::node> node) {
        const static std::unordered_map<std::string, QSizePolicy> map =
                tomlqt::detail::map::makeSizePolicy();

        return tomlqt::detail::tryGetEnumFromMap(node, map);
}
