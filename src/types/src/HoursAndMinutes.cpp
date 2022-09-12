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
#include "../include/whm/types/HoursAndMinutes.hpp"

#include "../include/whm/types/Time.hpp"

namespace whm {

namespace {

QString hoursAsString(int hours)
{
    return QString::number(hours);
}

QString minutesAsString(int minutes)
{
    return QStringLiteral("%1").arg(minutes, 2, 10, QLatin1Char('0'));
}

} // namespace

HoursAndMinutes::HoursAndMinutes(int hours, int minutes)
    : m_hours{(hours)}, m_minutes{(minutes)}
{
}

HoursAndMinutes::HoursAndMinutes(int minutes)
    : m_hours{minutes / 60}, m_minutes{minutes - m_hours * 60}
{
}

HoursAndMinutes::HoursAndMinutes(const Time &time)
    : HoursAndMinutes(time.totalMinutes())
{
}

QString HoursAndMinutes::toString() const
{
    auto hourString = hoursAsString(m_hours);
    auto minuteString = minutesAsString(m_minutes);
    QString resultString{hourString + ":" + minuteString};
    resultString.remove('-');
    if (m_minutes < 0 || m_hours < 0) {
        resultString.push_front('-');
    }
    return resultString;
}

Time HoursAndMinutes::toTime() const
{
    return Time{hours(), minutes()};
}

int HoursAndMinutes::hours() const
{
    return m_hours;
}

int HoursAndMinutes::minutes() const
{
    return m_minutes;
}

void HoursAndMinutes::setHours(int hours)
{
    if (m_hours == hours) {
        return;
    }
    m_hours = hours;
}

void HoursAndMinutes::setMinutes(int minutes)
{
    if (m_minutes == minutes) {
        return;
    }
    m_minutes = minutes;
}

HoursAndMinutes &HoursAndMinutes::operator+=(const HoursAndMinutes &rhs)
{
    auto minutesLhs = hours() * 60 + minutes();
    auto minutesRhs = rhs.hours() * 60 + rhs.minutes();
    auto minutes = minutesLhs + minutesRhs;
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    setHours(hours);
    setMinutes(minutes);
    return *this;
}

HoursAndMinutes &HoursAndMinutes::operator-=(const HoursAndMinutes &rhs)
{
    auto minutesLhs = hours() * 60 + minutes();
    auto minutesRhs = rhs.hours() * 60 + rhs.minutes();
    auto minutes = minutesLhs - minutesRhs;
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    setHours(hours);
    setMinutes(minutes);
    return *this;
}

bool operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs)
{
    if (lhs.hours() != rhs.hours()) {
        return false;
    }
    if (lhs.minutes() != rhs.minutes()) {
        return false;
    }
    return true;
}

bool operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs)
{
    return !(lhs == rhs);
}

HoursAndMinutes operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs)
{
    lhs += rhs;
    return lhs;
}

HoursAndMinutes operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs)
{
    lhs -= rhs;
    return lhs;
}

} // namespace whm
