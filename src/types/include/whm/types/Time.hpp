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
#ifndef WORKING_HOUR_MANAGER_TIME_HPP
#define WORKING_HOUR_MANAGER_TIME_HPP

#include <QTime>

namespace whm {

class Time {
public:
    Time();

    explicit Time(const QString &hhmmStr);
    Time(int hour, int minute);
    explicit Time(int minutes);

    Time(const Time &) = default;
    Time(Time &&) = default;
    Time &operator=(const Time &) = default;
    Time &operator=(Time &&) = default;

    int hour() const;

    int minute() const;

    QString asString() const;

    bool set(const QString &timeAsString);

    bool isValid() const;

    // time only represented with minutes
    int totalMinutes() const;

    Time &operator+=(const Time &rhs);
    Time &operator-=(const Time &rhs);

private:
    QTime m_time{};
};

bool operator==(const Time &lhs, const Time &rhs);
bool operator!=(const Time &lhs, const Time &rhs);

Time operator+(Time lhs, const Time &rhs);
Time operator-(Time lhs, const Time &rhs);

bool operator<(const Time &lhs, const Time &rhs);
bool operator>(const Time &lhs, const Time &rhs);
bool operator<=(const Time &lhs, const Time &rhs);
bool operator>=(const Time &lhs, const Time &rhs);

} // namespace whm

#endif
