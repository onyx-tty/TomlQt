// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include "CppUtils/String/StdString.h"
#include "TomlQt/Detail/QFlags.h"

#include <optional>
#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QDebug>
#include <QFlags>
#include <QString>
#include <QtGlobal>

template<typename T>
std::optional<QFlags<T>> tomlqt::detail::tryGetQFlagsFromMap(
        toml::node_view<const toml::node> node, const std::unordered_map<std::string, T>& map) {
        const auto* arr = node.as_array();
        if (!arr) { return std::nullopt; }

        QFlags<T> result;
        for (const auto& elem : *arr) {
                const auto key = elem.value<std::string>();
                if (!key) { return std::nullopt; }

                const auto iter = map.find(string::makeLower(*key));
                if (iter == map.cend()) {
                        qWarning() << QString("%1: Unknown flag value %2")
                                              .arg(__func__, QString::fromStdString(*key));
                        return std::nullopt;
                }

                result |= iter->second;
        }

        return result;
}
