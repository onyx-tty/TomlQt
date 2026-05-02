// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QSizePolicy>
#include <Qt>

namespace tomlqt {
namespace detail {

// TODO Document
template<typename T>
[[nodiscard]] std::optional<T> tryGetEnumFromMap(toml::node_view<const toml::node>         key,
                                                 const std::unordered_map<std::string, T>& map);

} // namespace detail
} // namespace tomlqt

#include "Enum.tpp"
