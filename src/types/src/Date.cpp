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
#include "../include/whm/types/Date.hpp"

#include <QMetaEnum>

namespace whm {

namespace {
QDate removeDay(QDate date)
{
    auto year = date.year();
    auto month = date.month();
    auto day = date.day();

    date.setDate(year, month, day - 1);

    // we were at start of the month so go back one month
    if (!date.isValid()) {
        date.setDate(year, month - 1, 1);
        auto daysInMonth = date.daysInMonth();
        date.setDate(year, month - 1, daysInMonth);

        // we were at start of the year so go back one year
        if (!date.isValid()) {
            date.setDate(year - 1, 12, 31);
        }
    }
    return date;
}

} // namespace

Date Date::currentDate()
{
    auto currentDateTime = QDateTime::currentDateTime();
    auto date = currentDateTime.date();
    return {date.year(), date.month(), date.day()};
}

Date::Date(int year, int month, int day) : m_date{year, month, day}
{
}

Date::Date(const QString date) : m_date{QDate::fromString(date, "dd.MM.yyyy")}
{
}

QString Date::asString() const
{
    return m_date.toString("dd.MM.yyyy");
}

Weekday Date::weekday() const
{
    auto weekdayAsString = m_date.toString("dddd");

    auto metaEnum = QMetaEnum::fromType<Weekday>();
    int weekdayAsInt = metaEnum.keyToValue(weekdayAsString.toLatin1());

    if (weekdayAsInt == -1) {
        return Weekday::Unknown;
    }
    return static_cast<Weekday>(weekdayAsInt);
}

int Date::month() const
{
    return m_date.month();
}

int Date::year() const
{
    return m_date.year();
}

Date Date::addDays(int days) const
{
    auto date = m_date.addDays(days);
    return {date.year(), date.month(), date.day()};
}

Date Date::removeDays(int days) const
{
    QDate returnDate = m_date;
    for (int i = 0; i < days; ++i) {
        returnDate = removeDay(returnDate);
    }
    return {returnDate.year(), returnDate.month(), returnDate.day()};
}

Date Date::getPreviouseDateWithWeekday(Weekday weekday) const
{
    Q_ASSERT(weekday != Weekday::Unknown);

    Date returnDate = *this;
    while (returnDate.weekday() != weekday) {
        returnDate = returnDate.removeDays(1);
    }
    return returnDate;
}

Date Date::getNextDateWithWeekday(Weekday weekday) const
{
    Q_ASSERT(weekday != Weekday::Unknown);

    Date returnDate = *this;
    while (returnDate.weekday() != weekday) {
        returnDate = returnDate.addDays(1);
    }
    return returnDate;
}

bool operator==(const Date &lhs, const Date &rhs)
{
    return lhs.asString() == rhs.asString();
}

bool operator!=(const Date &lhs, const Date &rhs)
{
    return !(lhs == rhs);
}

bool operator<(const Date &lhs, const Date &rhs)
{
    return lhs.m_date < rhs.m_date;
}

bool operator>(const Date &lhs, const Date &rhs)
{
    return rhs < lhs;
}

bool operator<=(const Date &lhs, const Date &rhs)
{
    return !(lhs > rhs);
}

bool operator>=(const Date &lhs, const Date &rhs)
{
    return !(lhs < rhs);
}

} // namespace whm
