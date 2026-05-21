// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>

namespace tomlqt::detail {

// TODO Document
template<typename T>
[[nodiscard]] std::optional<T> tryGetEnumFromMap(toml::node_view<const toml::node>         key,
                                                 const std::unordered_map<std::string, T>& map);

} // namespace tomlqt::detail

#include "Enum.tpp"
