// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/ArrayBounds.h"

#include <QDebug>
#include <QString>
#include <QtGlobal>

using validation_result = tomlqt::ArrayBounds::validation_result;

validation_result tomlqt::ArrayBounds::validate(const toml::array& arr) const {
        using enum validation_result;

        if (min_size && min_size > arr.size()) { return min_size_fail; }

        if (max_size && max_size < arr.size()) { return max_size_fail; }

        return success;
}

validation_result tomlqt::ArrayBounds::validate(const toml::array* arr) const {
        using enum validation_result;

        // Handles nullptr here, then delegates the rest to the const ref overload.
        if (!arr) { return null_ptr; }

        return validate(*arr);
}

void tomlqt::logArrayBoundsResult(tomlqt::ArrayBounds::validation_result result) {
        using validation_result = ArrayBounds::validation_result;
        using enum validation_result;

        switch (result) {
        // TODO: Create an ENABLE_ macro
        // TODO: Log success if that macro is set
        case success:       break;
        case min_size_fail: qWarning() << "toml::array size < min_size"; break;
        case max_size_fail: qWarning() << "toml::array size > max_size"; break;
        case null_ptr:      qWarning() << "Not constructible as toml::array"; break;
        default:            break;
        }
}
