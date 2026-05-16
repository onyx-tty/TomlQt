// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QFlags>
#include <Qt>

namespace tomlqt::detail {

// TODO Document
template<typename T>
[[nodiscard]] std::optional<QFlags<T>> tryGetQFlagsFromMap(
        toml::node_view<const toml::node> node, const std::unordered_map<std::string, T>& map);

} // namespace tomlqt::detail

#include "QFlags.tpp"
