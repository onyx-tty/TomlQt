// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "Helpers.h"

#include <cstdlib>
#include <iostream>
#include <string_view>
#include <toml++/toml.hpp>

toml::table getParsedTable(std::string_view example) {
        try {
                toml::table parsed = toml::parse(example);
                return parsed;
        } catch (const toml::parse_error& err) {
                std::cerr << "Parsing failed:\n" << err << "\n";
                exit(-1);
        }
}
