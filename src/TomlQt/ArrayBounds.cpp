// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/ArrayBounds.h"

#include <QDebug>
#include <QString>
#include <QtGlobal>

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
