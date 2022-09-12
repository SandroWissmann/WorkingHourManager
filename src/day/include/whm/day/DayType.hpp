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
#ifndef WORKING_HOUR_MANAGER_DAYTYPE_HPP
#define WORKING_HOUR_MANAGER_DAYTYPE_HPP

#include <QObject>
#include <QStringList>

namespace whm {

class DayTypeQEnum {
    Q_GADGET
public:
    enum class Value {
        Work,
        Holiday,
        Vacation,
        Sick,
        Party, // party at company generates the default work houts per day
        Flextime,
        Ignore,
    };
    Q_ENUM(Value)

private:
    explicit DayTypeQEnum();
};

typedef DayTypeQEnum::Value DayType;

QStringList dayTypeAsStringList();

} // namespace whm

#endif
