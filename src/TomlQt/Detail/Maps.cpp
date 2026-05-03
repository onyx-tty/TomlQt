// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Maps.h"

#include <string>
#include <unordered_map>
#include <QSizePolicy>
#include <Qt>

std::unordered_map<std::string, Qt::AlignmentFlag> tomlqt::detail::map::makeAlignment() {
        return {{"top", Qt::AlignTop},         {"center", Qt::AlignCenter},
                {"hcenter", Qt::AlignHCenter}, {"vcenter", Qt::AlignVCenter},
                {"bottom", Qt::AlignBottom},   {"left", Qt::AlignLeft},
                {"right", Qt::AlignRight}};
}

std::unordered_map<std::string, QSizePolicy> tomlqt::detail::map::makeSizePolicy() {
        return {{"fixed", {QSizePolicy::Fixed, QSizePolicy::Fixed}},
                {"minimum", {QSizePolicy::Minimum, QSizePolicy::Minimum}},
                {"maximum", {QSizePolicy::Maximum, QSizePolicy::Maximum}},
                {"preferred", {QSizePolicy::Preferred, QSizePolicy::Preferred}},
                {"expanding", {QSizePolicy::Expanding, QSizePolicy::Expanding}},
                {"minimumexpanding", {QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding}},
                {"minimum_expanding", {QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding}},
                {"ignored", {QSizePolicy::Ignored, QSizePolicy::Ignored}}};
}
