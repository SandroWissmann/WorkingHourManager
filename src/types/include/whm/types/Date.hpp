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
#ifndef WORKING_HOUR_MANAGER_DATE_HPP
#define WORKING_HOUR_MANAGER_DATE_HPP

#include "Weekday.hpp"

#include <QDate>

namespace whm {

class Date {
public:
    static Date currentDate();

    Date(int year, int month, int day);

    // dd.MM.yyyy
    Date(const QString date);

    Date(const Date &) = default;
    Date(Date &&) = default;
    Date &operator=(const Date &) = default;
    Date &operator=(Date &&) = default;

    // format dd.MM.yyyy
    QString asString() const;

    Weekday weekday() const;

    int month() const;

    int year() const;

    Date addDays(int days) const;

    Date removeDays(int days) const;

    Date getPreviouseDateWithWeekday(Weekday weekday) const;

    Date getNextDateWithWeekday(Weekday weekday) const;

private:
    QDate m_date;

    friend bool operator<(const Date &lhs, const Date &rhs);
};

bool operator==(const Date &lhs, const Date &rhs);
bool operator!=(const Date &lhs, const Date &rhs);

bool operator<(const Date &lhs, const Date &rhs);
bool operator>(const Date &lhs, const Date &rhs);
bool operator<=(const Date &lhs, const Date &rhs);
bool operator>=(const Date &lhs, const Date &rhs);

} // namespace whm

#endif
