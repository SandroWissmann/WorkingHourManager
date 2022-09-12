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
#include "../include/whm/day/HelperDay.hpp"

#include "../include/whm/day/Day.hpp"

#include <whm/types/Date.hpp>

namespace whm {

namespace {
Date calcStartDate(const Date &date)
{
    Q_ASSERT(date.weekday() != Weekday::Unknown);

    switch (date.weekday()) {
        case Weekday::Monday:
            return date;
        case Weekday::Tuesday:
        case Weekday::Wednesday:
        case Weekday::Thursday:
        case Weekday::Friday:
            return date.getPreviouseDateWithWeekday(Weekday::Monday);
        case Weekday::Saturday:
        case Weekday::Sunday:
            return date.getNextDateWithWeekday(Weekday::Monday);
        case Weekday::Unknown:
            // case cannot be hit. just here to silence compiler.
            return date;
    }
    return date;
}

void addDayOnWorkday(
    QVector<std::shared_ptr<Day>> &days,
    Weekday weekday,
    const Date &date)
{
    switch (weekday) {
        case Weekday::Monday:
        case Weekday::Tuesday:
        case Weekday::Wednesday:
        case Weekday::Thursday:
        case Weekday::Friday: {
            auto day = std::make_shared<Day>(date);
            days.emplace_back(day);
        } break;
        case Weekday::Saturday:
            break;
        case Weekday::Sunday:
            break;
        case Weekday::Unknown:
            break;
    }
}
} // namespace

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate)
{
    Q_ASSERT(firstDate < lastDate);

    auto weekday{Weekday::Monday};

    auto date = calcStartDate(firstDate);
    auto endDate = lastDate.getNextDateWithWeekday(Weekday::Friday);

    Q_ASSERT(date <= endDate);

    QVector<std::shared_ptr<Day>> days;
    while (date <= endDate) {
        auto weekday = date.weekday();
        addDayOnWorkday(days, weekday, date);
        date = date.addDays(1);
    }
    return days;
}
} // namespace whm
