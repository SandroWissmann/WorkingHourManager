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
#ifndef WORKING_HOUR_MANAGER_DAY_HPP
#define WORKING_HOUR_MANAGER_DAY_HPP

#include "DayType.hpp"

#include <whm/types/Date.hpp>
#include <whm/types/Time.hpp>

namespace whm {

class Day {
public:
    Day(const Date &date,
        const Time &startTime,
        const Time &endTime,
        DayType dayType);

    Day(const Date &date, const Time &startTime, const Time &endTime);

    explicit Day(const Date &date);

    Day(const Day &) = default;
    Day(Day &&) = default;
    Day &operator=(const Day &) = default;
    Day &operator=(Day &&) = default;

    bool isValid() const;

    Date date() const;

    Time startTime() const;
    bool setStartTime(const Time &startTime);
    bool setStartTime(const QString &startTimeAsString);
    bool hasValidStartTime() const;

    Time endTime() const;
    bool setEndTime(Time endTime);
    bool setEndTime(const QString &endTimeAsString);
    bool hasValidEndTime() const;

    DayType dayType() const;
    bool setDayType(DayType dayType);

private:
    Date m_date;
    Time m_startTime;
    Time m_endTime;
    DayType m_dayType{DayType::Work};
};
} // namespace whm

#endif
