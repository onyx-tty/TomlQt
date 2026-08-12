// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#pragma once

#include <cstddef>
#include <optional>
#include <toml++/toml.hpp>
#include <QDebug>
#include <QString>
#include <QtGlobal>

namespace tomlqt {

// TODO: Test .validate()
// TODO: Document
struct ArrayBounds final {
        std::optional<size_t> min_size = std::nullopt;
        std::optional<size_t> max_size = std::nullopt;
        enum class validation_result { success, min_size_fail, max_size_fail, null_ptr };

        // Validate and check if:
        // 1. Size of potential toml::array NOT HIGHER THAN max_size.
        // 2. Size of potential toml::array NOT LOWER THAN min_size.
        //
        // Return ArrayBounds::validation_result.
        //
        // Bound-checking is skipped for the attribute (max_size or min_size) that is nullopt.
        [[nodiscard]] validation_result validate(const toml::array& arr) const;

        // Validate and check if:
        // 1. Passed toml::array NOT NULL.
        // 2. Size of potential toml::array NOT HIGHER THAN max_size.
        // 3. Size of potential toml::array NOT LOWER THAN min_size.
        //
        // Return ArrayBounds::validation_result.
        //
        // Bound-checking is skipped for the attribute (max_size or min_size) that is nullopt.
        [[nodiscard]] validation_result validate(const toml::array* arr) const;
};

void logArrayBoundsResult(ArrayBounds::validation_result result);

} // namespace tomlqt
