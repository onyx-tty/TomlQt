// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>

namespace tomlqt {

// TODO: Test .validate()
struct ArrayBounds final {
        std::optional<size_t> min_size = std::nullopt;
        std::optional<size_t> max_size = std::nullopt;
        enum class validation_result { success, min_size_fail, max_size_fail, null_ptr };

        [[nodiscard]] validation_result validate(const toml::array& arr) const {
                using enum validation_result;

                if (min_size && min_size > arr.size()) { return min_size_fail; }
                if (max_size && max_size < arr.size()) { return max_size_fail; }
                return success;
        }

        [[nodiscard]] validation_result validate(const toml::array* arr) const {
                using enum validation_result;

                if (!arr) { return null_ptr; }
                if (min_size && min_size > arr->size()) { return min_size_fail; }
                if (max_size && max_size < arr->size()) { return max_size_fail; }
                return success;
        }
};

} // namespace tomlqt
