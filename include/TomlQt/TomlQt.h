// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <concepts>
#include <cstdint>
#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <Qt>

class QSize;
class QSizePolicy;
class QString;

// TODO Document std::nullopt return paths

namespace tomlqt {

template<typename T> concept TomlSupported = std::same_as<T, std::string>
                                          || std::same_as<T, toml::date>
                                          || std::same_as<T, toml::time>
                                          || std::same_as<T, toml::date_time>
                                          || std::same_as<T, std::int64_t>
                                          || std::same_as<T, double> || std::same_as<T, bool>;

template<typename T> concept ValueConvertible = std::same_as<T, QString> || std::same_as<T, QSize>
                                             || std::same_as<T, QSizePolicy>
                                             || std::same_as<T, Qt::Alignment> || TomlSupported<T>;

// Currently supported specializations:
// - QString
// - QSize
// - QSizePolicy
// - Qt::Alignment
template<ValueConvertible TReturn>
[[nodiscard]] std::optional<TReturn> value(toml::node_view<const toml::node> node);

// Get std::optional<QString> from toml::value<std::string>.
//
// Format: toml::value<std::string>
//
// Example: foo = "TomlQt"
template<>
std::optional<QString> value<QString>(toml::node_view<const toml::node> node);

// Get std::optional<QSize> from toml::array of toml::value<int64_t> values [width, height].
// Numbers beyond the first 2 are ignored.
//
// Format: toml::array of toml::value<int64_t> values [width, height]
//
// Example: foo = [100, 50]
template<>
std::optional<QSize> value<QSize>(toml::node_view<const toml::node> node);

// Get std::optional<QSizePolicy> from toml::value<std::string>.
// Applies the same policy to both width and height.
// Case-insensitive.
//
// Supports: "Fixed", "Minimum", "Maximum", "Preferred", "Expanding",
//           "MinimumExpanding" (or "Minimum_Expanding"), "Ignored"
//
// Examples:
// foo = "fixed"
// foo = "MinimumExpanding"
// foo = "minimum_expanding"
// TODO Separate height and width
template<>
std::optional<QSizePolicy> value<QSizePolicy>(toml::node_view<const toml::node> node);

// Get std::optional<Qt::Alignment> from toml::value<std::string>.
// Case-insensitive.
//
// Format:
// a) Pure Qt::Alignment: toml::value<std::string>
//
//   Supports: "Top", "Bottom", "Left", "Right", "Center"
//
//   Examples:
//   foo = "top"     // For Qt::Alignment{Qt::AlignHCenter | Qt::AlignTop}
//   foo = "left"    // For Qt::Alignment{Qt::AlignLeft | Qt::AlignVCenter}
//   foo = "center"  // For Qt::Alignment{Qt::AlignCenter}
//
// b) QFlags: toml::array of toml::value<std::string>
//
//   Supports: "Top", "VCenter", "Bottom", "HCenter", "Left", "Right", "Center"
//
//   Examples:
//   foo = ["top", "left"]       // For Qt::Alignment{Qt::AlignTop, Qt::AlignLeft}
//   foo = ["top"]               // For Qt::Alignment{Qt::AlignTop}
//   foo = ["vcenter", "right"]  // For Qt::Alignment{Qt::AlignVCenter, Qt::AlignRight}
template<>
std::optional<Qt::Alignment> value<Qt::Alignment>(toml::node_view<const toml::node> node);

// Forwards other types to toml::value() if supported.
//
// Supports types: std::string, toml::date, toml::time, toml::date_time,
//                 std::int64_t, double, bool
template<TomlSupported T>
std::optional<T> value(toml::node_view<const toml::node> node) {
        return node.value<T>();
}

} // namespace tomlqt
