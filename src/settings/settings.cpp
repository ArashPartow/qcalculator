/**
 *  QCalc - Extensible programming calculator
 *  Copyright (C) 2021  Julian Zampiccoli
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "settings/settings.hpp"

#include "io/paths.hpp"
#include "io/serializer.hpp"
#include "io/fileoperations.hpp"

#include <QFile>

Settings Settings::readSettings() {
    auto path = Paths::getSettingsFile();
    if (QFile(path.c_str()).exists()) {
        return Serializer::deserializeSettings(FileOperations::fileReadAllText(path));
    } else {
        return {};
    }
}

void Settings::saveSettings(const Settings &settings) {
    return FileOperations::fileWriteAllText(Paths::getSettingsFile(), Serializer::serializeSettings(settings));
}

void Settings::clear(const Setting &s) {
    data.erase(s.key);
}

const Settings::Entry &Settings::value(const Setting &setting) const {
    if (data.find(setting.key) == data.end())
        return setting.entry;
    else
        return data.at(setting.key);
}
