// SPDX-FileCopyrightText: 2026 Łukasz Wrodarczyk
// SPDX-License-Identifier: MIT

#include "TomlQt/Detail/Maps.h"

#include <string>
#include <unordered_map>
#include <QSizePolicy>
#include <Qt>

std::unordered_map<std::string, Qt::Alignment> tomlqt::detail::map::makeAlignment() {
        return {{"top", Qt::AlignHCenter | Qt::AlignTop},
                {"center", Qt::AlignCenter},
                {"bottom", Qt::AlignHCenter | Qt::AlignBottom},
                {"left", Qt::AlignLeft | Qt::AlignVCenter},
                {"right", Qt::AlignRight | Qt::AlignVCenter}};
}

std::unordered_map<std::string, Qt::AlignmentFlag> tomlqt::detail::map::makeAlignmentFlag() {
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
