/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
#include "../include/whm/day/DayType.hpp"

#include <QMetaEnum>

namespace whm {

QStringList dayTypeAsStringList()
{
    auto metaEnum = QMetaEnum::fromType<DayType>();
    QStringList dayTypesAsStrings;

    int count = metaEnum.keyCount();

    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        auto dayType = static_cast<DayType>(metaEnum.value(i));

        auto dayTypeAsString = QVariant::fromValue(dayType).toString();
        dayTypesAsStrings.emplace_back(dayTypeAsString);
    }
    return dayTypesAsStrings;
}
} // namespace whm
