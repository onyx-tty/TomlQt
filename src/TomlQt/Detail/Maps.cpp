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
        using enum QSizePolicy::Policy;

        return {{"fixed", {Fixed, Fixed}},
                {"minimum", {Minimum, Minimum}},
                {"maximum", {Maximum, Maximum}},
                {"preferred", {Preferred, Preferred}},
                {"expanding", {Expanding, Expanding}},
                {"minimumexpanding", {MinimumExpanding, MinimumExpanding}},
                {"minimum_expanding", {MinimumExpanding, MinimumExpanding}},
                {"ignored", {Ignored, Ignored}}};
}
