// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include "CppUtils/String/StdString.h"
#include "TomlQt/Detail/Enum.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QDebug>
#include <QString>
#include <QtGlobal>

template<typename T>
std::optional<T> tomlqt::detail::tryGetEnumFromMap(toml::node_view<const toml::node>         key,
                                                   const std::unordered_map<std::string, T>& map) {
        const auto key_str = key.value<std::string>();
        if (!key_str) { return std::nullopt; }

        const auto iter = map.find(string::makeLower(key_str.value()));
        if (iter == map.cend()) {
                qWarning() << QString("%1: Key %2 not found in map!")
                                      .arg(__func__, QString::fromStdString(key_str.value()));
                return std::nullopt;
        }

        return iter->second;
}
