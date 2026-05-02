// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include "CppUtils/Log/QtLog.h"
#include "CppUtils/String/StdString.h"
#include "Detail/Enum.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QString>

template<typename T>
std::optional<T> tomlqt::detail::tryGetEnumFromMap(toml::node_view<const toml::node>         key,
                                                   const std::unordered_map<std::string, T>& map) {
        const auto key_str = key.value<std::string>();
        if (!key_str) { return std::nullopt; }

        const auto iter = map.find(string::toLowerCopy(key_str.value()));
        if (iter == map.cend()) {
                QWARNING() << QString("Key %1 not found in map!")
                                      .arg(QString::fromStdString(key_str.value()));
                return std::nullopt;
        }

        return iter->second;
}
