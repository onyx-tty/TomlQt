// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include "CppUtils/Log/QtLog.h"
#include "CppUtils/String/StdString.h"
#include "TomlQt/Detail/QFlags.h"

#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QFlags>
#include <QString>

template<typename T>
std::optional<QFlags<T>> tomlqt::detail::tryGetQFlagsFromMap(
        toml::node_view<const toml::node> node, const std::unordered_map<std::string, T>& map) {
        const auto* arr = node.as_array();
        if (!arr) { return std::nullopt; }

        QFlags<T> result;
        for (const auto& elem : *arr) {
                const auto key = elem.value<std::string>();
                if (!key) { return std::nullopt; }

                const auto iter = map.find(string::toLowerCopy(*key));
                if (iter == map.cend()) {
                        QWARNING() << QString("Unknown flag value %1")
                                              .arg(QString::fromStdString(*key));
                        return std::nullopt;
                }

                result |= iter->second;
        }

        return result;
}
