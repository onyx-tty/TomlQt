// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <string>
#include <toml++/toml.hpp>
#include <unordered_map>
#include <QSizePolicy>
#include <Qt>

namespace tomlqt {
namespace detail {
namespace map {

[[nodiscard]] std::unordered_map<std::string, Qt::Alignment>     makeAlignment();
[[nodiscard]] std::unordered_map<std::string, Qt::AlignmentFlag> makeAlignmentFlag();
[[nodiscard]] std::unordered_map<std::string, QSizePolicy>       makeSizePolicy();

} // namespace map
} // namespace detail
} // namespace tomlqt
