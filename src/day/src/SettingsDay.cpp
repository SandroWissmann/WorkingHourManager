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
#include "../include/whm/day/SettingsDay.hpp"

namespace whm {

SettingsDay::SettingsDay(Weekday weekday, Time defaultWorkTime, Time pauseTime)
    : m_weekday{weekday}, m_defaultWorkTime{defaultWorkTime}, m_pauseTime{
                                                                  pauseTime}
{
}

Weekday SettingsDay::weekday() const
{
    return m_weekday;
}

QString SettingsDay::weekdayAsString() const
{
    QString weekday = QVariant::fromValue(m_weekday).toString();
    weekday[0] = weekday[0].toUpper();
    return weekday;
}

Time SettingsDay::defaultWorkTime() const
{
    return m_defaultWorkTime;
}

bool SettingsDay::setDefaultWorkTime(const QString &defaultWorkTime)
{
    if (m_defaultWorkTime.asString() == defaultWorkTime) {
        return false;
    }
    m_defaultWorkTime.set(defaultWorkTime);
    return true;
}

Time SettingsDay::pauseTime() const
{
    return m_pauseTime;
}

bool SettingsDay::setPauseTime(const QString &pauseTime)
{
    if (m_pauseTime.asString() == pauseTime) {
        return false;
    }
    m_pauseTime.set(pauseTime);
    return true;
}

} // namespace whm
