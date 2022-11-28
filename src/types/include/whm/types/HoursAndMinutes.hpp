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
#ifndef WORKING_HOUR_MANAGER_HOURSANDMINUTES_HPP
#define WORKING_HOUR_MANAGER_HOURSANDMINUTES_HPP

#include <QDateTime>

namespace whm {

class Time;

class HoursAndMinutes {
public:
    HoursAndMinutes() = default;
    explicit HoursAndMinutes(int minutes);
    explicit HoursAndMinutes(const Time &time);
    HoursAndMinutes(int hours, int minutes);

    HoursAndMinutes(const HoursAndMinutes &) = default;
    HoursAndMinutes(HoursAndMinutes &&) = default;
    HoursAndMinutes &operator=(const HoursAndMinutes &) = default;
    HoursAndMinutes &operator=(HoursAndMinutes &&) = default;

    // format hh:mm
    QString toString() const;

    // format hh,mm
    QString toFractionString() const;

    // If HoursAndMinutes will be > 23:59 time will be invalid
    Time toTime() const;

    int hours() const;
    int minutes() const;

    HoursAndMinutes &operator+=(const HoursAndMinutes &rhs);
    HoursAndMinutes &operator-=(const HoursAndMinutes &rhs);

private:
    void setHours(int hours);
    void setMinutes(int minutes);

    int m_hours{};
    int m_minutes{};

    friend bool
    operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);
    friend bool
    operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);

    friend HoursAndMinutes
    operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
    friend HoursAndMinutes
    operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
};

bool operator==(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);
bool operator!=(const HoursAndMinutes &lhs, const HoursAndMinutes &rhs);

HoursAndMinutes operator+(HoursAndMinutes lhs, const HoursAndMinutes &rhs);
HoursAndMinutes operator-(HoursAndMinutes lhs, const HoursAndMinutes &rhs);

} // namespace whm

#endif
