// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <string_view>
#include <toml++/toml.hpp>

[[nodiscard]] toml::table getParsedTable(std::string_view example);
