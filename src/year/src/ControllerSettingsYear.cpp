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
#include "../include/whm/year/ControllerSettingsYear.hpp"

#include <whm/day/ControllerSettingsDay.hpp>

namespace whm {

namespace {

QVector<QObject *> makeControllerSettingsDays(SettingsYear &settingsYear)
{
    return QVector<QObject *>{
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Monday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Tuesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Wednesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Thursday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Friday)}};
}

} // namespace

ControllerSettingsYear::ControllerSettingsYear(const SettingsYear &settingsYear)
    : m_settingsYear(settingsYear),
      m_controllerSettingsDays{makeControllerSettingsDays(m_settingsYear)}
{
}

SettingsYear ControllerSettingsYear::settingsYear() const
{
    return m_settingsYear;
}

QVector<QObject *> ControllerSettingsYear::controllerSettingsDays() const
{
    return m_controllerSettingsDays;
}

double ControllerSettingsYear::flextimeDays()
{
    return m_settingsYear.flextimeDays();
}

QString ControllerSettingsYear::flextimeDaysAsString()
{
    return QString::number(flextimeDays(), 'f', 1);
}

void ControllerSettingsYear::setFlextimeDays(
    const QString &flextimeDaysAsString)
{
    if (!m_settingsYear.setFlextimeDays(flextimeDaysAsString)) {
        return;
    }
    emit flextimeDaysChanged();
}

double ControllerSettingsYear::vacationDays()
{
    return m_settingsYear.vacationDays();
}

QString ControllerSettingsYear::vacationDaysAsString()
{
    return QString::number(vacationDays(), 'f', 1);
}

void ControllerSettingsYear::setVacationDays(
    const QString &vacationDaysAsString)
{
    if (!m_settingsYear.setVacationDays(vacationDaysAsString)) {
        return;
    }
    emit vacationDaysChanged();
}

} // namespace whm
