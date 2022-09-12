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
#include "ControllerDayStateFlextime.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {

ControllerDayState &ControllerDayStateFlextime::getInstance()
{
    static ControllerDayStateFlextime instance;
    return instance;
}

void ControllerDayStateFlextime::calculate(ControllerDay *controllerDay)
{
    controllerDay->setStartTime("0:00");
    controllerDay->setEndTime("0:00");

    auto defaultWorkTime = controllerDay->defaultWorkTime();
    controllerDay->setWorkTime(defaultWorkTime);

    auto overtime = HoursAndMinutes{-defaultWorkTime.totalMinutes()};
    controllerDay->setOvertime(overtime);

    controllerDay->setTimeInputIsEnabled(false);
    controllerDay->setUsedFlextimeDay(1.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
