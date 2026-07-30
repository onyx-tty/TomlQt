// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/TomlQt.h"
#include "TomlQt/ArrayBounds.h"
#include "TomlQt/Detail/Enum.h"
#include "TomlQt/Detail/Maps.h"
#include "TomlQt/Detail/QFlags.h"

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QDebug>
#include <QFlag>
#include <QSize>
#include <QSizePolicy>
#include <QString>
#include <Qt>
#include <QtGlobal>

template<>
std::optional<QString> tomlqt::value<QString>(toml::node_view<const toml::node> node) {
        if (auto str = node.value<std::string>()) { return QString::fromStdString(str.value()); }

        return std::nullopt;
}

template<>
std::optional<QSize> tomlqt::value<QSize>(toml::node_view<const toml::node> node) {
        using result              = tomlqt::ArrayBounds::validation_result;
        const auto arr_conditions = tomlqt::ArrayBounds{.min_size = 2};

        const auto* arr = asArrayWithBounds(node, arr_conditions);
        if (!arr) { return std::nullopt; }

        auto width  = (*arr)[0].value<int64_t>();
        auto height = (*arr)[1].value<int64_t>();

        if (!width || !height) {
                qWarning() << QString("%1: QSize requires 2 numbers for construction. width: %2, height: %3")
                                      .arg(__func__)
                                      .arg(width ? "OK" : "NULL")
                                      .arg(height ? "OK" : "NULL");
                return std::nullopt;
        }

        // TODO Handle narrowing conversion
        return QSize(width.value(), height.value());
}

template<>
std::optional<QSizePolicy> tomlqt::value<QSizePolicy>(toml::node_view<const toml::node> node) {
        const static std::unordered_map<std::string, QSizePolicy> map =
                tomlqt::detail::map::makeSizePolicy();

        return tomlqt::detail::tryGetEnumFromMap(node, map);
}

template<>
std::optional<Qt::Alignment> tomlqt::value<Qt::Alignment>(toml::node_view<const toml::node> node) {
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

#define INSTANTIATE(T) \
        template std::optional<T> tomlqt::value<T>(toml::node_view<const toml::node> node);

INSTANTIATE(std::string)
INSTANTIATE(toml::date)
INSTANTIATE(toml::time)
INSTANTIATE(toml::date_time)
INSTANTIATE(std::int64_t)
INSTANTIATE(double)
INSTANTIATE(bool)

#undef INSTANTIATE

const toml::array* tomlqt::asArrayWithBounds(toml::node_view<const toml::node> node,
                                             const ArrayBounds&                bounds) {
        using result           = tomlqt::ArrayBounds::validation_result;
        const toml::array* arr = node.as_array();

        const auto serialize_size = [](std::optional<size_t> size) -> QString {
                return size ? QString::number(size.value()) : "NULL";
        };

        switch (auto res = bounds.validate(arr)) {
        case result::success: return arr;
        case result::null_ptr:
                if (node.type() == toml::node_type::none) {
                        qWarning() << "Passed node is empty";
                } else {
                        qWarning() << "Cannot parse as array";
                }

                return nullptr;
        case result::min_size_fail:
                qWarning() << QString("toml::array size < min_size! min_size: %1, arr size: %2")
                                      .arg(serialize_size(bounds.min_size),
                                           QString::number(arr->size()));

                return nullptr;
        case result::max_size_fail:
                qWarning() << QString("toml::array size > max_size! max_size: %1, arr size: %2")
                                      .arg(serialize_size(bounds.max_size),
                                           QString::number(arr->size()));

                return nullptr;
        default: qFatal("Unhandled case %i", static_cast<int>(res));
        }
}
