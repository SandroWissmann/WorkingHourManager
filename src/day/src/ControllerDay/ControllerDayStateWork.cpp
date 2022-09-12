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
#include "ControllerDayStateWork.hpp"

#include "../../include/whm/day/ControllerDay.hpp"

namespace whm {

namespace {
Time calcWorkTime(
    const Time &startTime,
    const Time &endTime,
    const Time &pauseTime)
{
    auto timesAreValid = startTime.isValid() && endTime.isValid();

    if (!timesAreValid) {
        return Time{};
    }
    auto workTime = endTime - startTime - pauseTime;

    // If result would be negative time gets invalid and has empty string so
    // we correct it to 0:00
    if (workTime.asString().isEmpty()) {
        return Time{};
    }
    return workTime;
}

HoursAndMinutes calcOvertime(
    const Time &startTime,
    const Time &endTime,
    const Time &workTime,
    const Time &defaultWorkTime)
{
    if (!startTime.isValid()) {
        return HoursAndMinutes{};
    }
    if (!endTime.isValid()) {
        return HoursAndMinutes{};
    }
    return HoursAndMinutes{workTime} - HoursAndMinutes{defaultWorkTime};
}

} // namespace

ControllerDayState &ControllerDayStateWork::getInstance()
{
    static ControllerDayStateWork instance;
    return instance;
}

void ControllerDayStateWork::calculate(ControllerDay *controllerDay)
{
    auto enteredStartTime = controllerDay->enteredStartTime();
    controllerDay->setStartTime(enteredStartTime);

    auto enteredEndTime = controllerDay->enteredEndTime();
    controllerDay->setEndTime(enteredEndTime);

    auto workTime = calcWorkTime(
        controllerDay->startTime(),
        controllerDay->endTime(),
        controllerDay->pauseTime());
    controllerDay->setWorkTime(workTime);

    auto startTime = controllerDay->startTime();
    auto endTime = controllerDay->endTime();
    auto defaultWorkTime = controllerDay->defaultWorkTime();

    auto overtime = calcOvertime(startTime, endTime, workTime, defaultWorkTime);
    controllerDay->setOvertime(overtime);

    controllerDay->setTimeInputIsEnabled(true);

    controllerDay->setUsedFlextimeDay(0.0);
    controllerDay->setUsedVacationDay(0.0);
}

} // namespace whm
