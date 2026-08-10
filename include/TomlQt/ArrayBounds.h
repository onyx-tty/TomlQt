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
        // 1. Size of potential toml::array NOT HIGHER THAN provided bounds.max_size.
        // 2. Size of potential toml::array NOT LOWER THAN provided bounds.min_size.
        //
        // Return ArrayBounds::validation_result.
        //
        // Pass 'log = true' to enable logging, it's disabled by default.
        //
        // Bound-checking is skipped for the attribute (max_size or min_size) that is nullopt.
        [[nodiscard]] validation_result validate(const toml::array& arr, bool log = false) const {
                using enum validation_result;

                static const auto serialize_size = [](std::optional<size_t> size) -> QString {
                        return size ? QString::number(size.value()) : "NULL";
                };

                if (min_size && min_size > arr.size()) {
                        qWarning()
                                << QString("toml::array size < min_size! min_size: %1, arr size: %2")
                                           .arg(serialize_size(min_size),
                                                QString::number(arr.size()));
                        return min_size_fail;
                }

                if (max_size && max_size < arr.size()) {
                        qWarning()
                                << QString("toml::array size > max_size! max_size: %1, arr size: %2")
                                           .arg(serialize_size(max_size),
                                                QString::number(arr.size()));
                        return max_size_fail;
                }

                // TODO: Create an ENABLE_ macro
                // TODO: Log success if that macro is set
                return success;
        }

        [[nodiscard]] validation_result validate(const toml::array* arr, bool log = false) const {
                using enum validation_result;

                // Handles nullptr here, then delegates the rest to the const ref overload.
                if (!arr) {
                        qWarning() << "Not constructible as toml::array";
                        return null_ptr;
                }

                return validate(*arr);
        }
};

} // namespace tomlqt
