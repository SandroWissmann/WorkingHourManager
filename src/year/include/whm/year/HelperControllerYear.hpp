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
#ifndef WORKING_HOUR_MANAGER_HELPER_CONTROLLER_YEAR_HPP
#define WORKING_HOUR_MANAGER_HELPER_CONTROLLER_YEAR_HPP

#include <QObject>
#include <QVector>

namespace whm {

class Day;
class SettingsYear;

void debugPrint(const QVector<QObject *> &controllerYears);

/*
  Steps:

  1 Generate ControllerDays from Days and SettingsYears

  2 Generate ControllerWeeks from ControllerDays

  3 Get first year from Days

  4 Generate ControllerMonths from first year + ControllerWeeks

  5 Generate ControllerSettingsYears from SettingsYears

  6 Generate Controller Years from ControllerMonths and ControllerSettingsYears
*/
QVector<QObject *> makeControllerYears(
    const QVector<std::shared_ptr<Day>> &days,
    const std::map<int, SettingsYear> &yearsToSettingsYears);

QVector<std::shared_ptr<Day>>
getDays(const QVector<QObject *> &controllerYears);

QVector<SettingsYear>
getSettingsYears(const QVector<QObject *> &controllerYears);

} // namespace whm

#endif
