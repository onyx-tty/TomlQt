// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>

namespace tomlqt {

struct ArrayBounds final {
        std::optional<size_t> min_size = std::nullopt;
        std::optional<size_t> max_size = std::nullopt;
        enum class validation_result { success, min_size_fail, max_size_fail };

        [[nodiscard]] validation_result validate(const toml::array& arr) const {
                if (min_size && min_size > arr.size()) { return validation_result::min_size_fail; }
                if (max_size && max_size < arr.size()) { return validation_result::max_size_fail; }
                return validation_result::success;
        }
};

} // namespace tomlqt
