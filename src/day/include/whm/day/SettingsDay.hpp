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
#ifndef WORKING_HOUR_MANAGER_SETTINGS_DAY_HPP
#define WORKING_HOUR_MANAGER_SETTINGS_DAY_HPP

#include <whm/types/Time.hpp>
#include <whm/types/Weekday.hpp>

#include <map>

namespace whm {

class SettingsDay {
public:
    SettingsDay(Weekday weekday, Time defaultWorkTime, Time pauseTime);

    SettingsDay(const SettingsDay &) = default;
    SettingsDay(SettingsDay &&) = default;
    SettingsDay &operator=(const SettingsDay &) = default;
    SettingsDay &operator=(SettingsDay &&) = default;

    Weekday weekday() const;
    QString weekdayAsString() const;

    Time defaultWorkTime() const;
    bool setDefaultWorkTime(const QString &defaultWorkTime);

    Time pauseTime() const;
    bool setPauseTime(const QString &pauseTime);

private:
    Weekday m_weekday;
    Time m_defaultWorkTime;
    Time m_pauseTime;
};

} // namespace whm

#endif
