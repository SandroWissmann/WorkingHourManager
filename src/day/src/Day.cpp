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
#include "../include/whm/day/Day.hpp"

namespace whm {

namespace {
bool hasValidTime(const Time &time, DayType dayType)
{
    if (dayType != DayType::Work) {
        return true;
    }
    if (!time.isValid()) {
        return false;
    }
    return true;
}
} // namespace

Day::Day(
    const Date &date,
    const Time &startTime,
    const Time &endTime,
    DayType dayType)
    : m_date{date}, m_startTime{startTime},
      m_endTime(endTime), m_dayType{dayType}
{
}

Day::Day(const Date &date, const Time &startTime, const Time &endTime)
    : Day(date, startTime, endTime, DayType::Work)
{
}

Day::Day(const Date &date) : Day(date, Time{}, Time{}, DayType::Work)
{
}

bool Day::isValid() const
{
    if (m_dayType != DayType::Work) {
        return true;
    }
    if (!m_startTime.isValid()) {
        return false;
    }
    if (!m_endTime.isValid()) {
        return false;
    }
    if (m_startTime < m_endTime) {
        return true;
    }
    return false;
}

Date Day::date() const
{
    return m_date;
}

Time Day::startTime() const
{
    if (m_dayType != DayType::Work) {
        return Time{};
    }
    return m_startTime;
}
bool Day::setStartTime(const Time &startTime)
{
    if (m_startTime == startTime) {
        return false;
    }
    m_startTime = startTime;
    return true;
}

bool Day::setStartTime(const QString &startTimeAsString)
{
    return m_startTime.set(startTimeAsString);
}

bool Day::hasValidStartTime() const
{
    if (hasValidTime(startTime(), dayType())) {
        return true;
    }
    return false;
}

Time Day::endTime() const
{
    if (m_dayType != DayType::Work) {
        return Time{};
    }
    return m_endTime;
}

bool Day::setEndTime(Time endTime)
{
    // detect input as PM and transform to to 24h format.
    if (endTime.isValid() && endTime < m_startTime && endTime.hour() < 12) {
        auto hour = endTime.hour();
        auto minute = endTime.minute();
        hour += 12;
        endTime = Time{hour, minute};
    }

    if (m_endTime == endTime) {
        return false;
    }
    m_endTime = endTime;
    return true;
}

bool Day::setEndTime(const QString &endTimeAsString)
{
    Time endTime;
    endTime.set(endTimeAsString);
    return setEndTime(endTime);
}

bool Day::hasValidEndTime() const
{
    if (hasValidTime(endTime(), dayType())) {
        return true;
    }
    return false;
}

DayType Day::dayType() const
{
    return m_dayType;
}

bool Day::setDayType(DayType dayType)
{
    if (m_dayType == dayType) {
        return false;
    }
    m_dayType = dayType;
    return true;
}

} // namespace whm
