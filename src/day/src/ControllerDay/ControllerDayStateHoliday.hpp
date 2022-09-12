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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HOLIDAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HOLIDAY_HPP

#include "ControllerDayState.hpp"
namespace whm {

class ControllerDayStateHoliday : public ControllerDayState {
public:
    ControllerDayStateHoliday() = default;
    virtual ~ControllerDayStateHoliday() override = default;

    ControllerDayStateHoliday(const ControllerDayStateHoliday &) = default;
    ControllerDayStateHoliday(ControllerDayStateHoliday &&) = default;
    ControllerDayStateHoliday &
    operator=(const ControllerDayStateHoliday &) = default;
    ControllerDayStateHoliday &
    operator=(ControllerDayStateHoliday &&) = default;

    static ControllerDayState &getInstance();
    void calculate(ControllerDay *controllerDay) override;
};

} // namespace whm

#endif
