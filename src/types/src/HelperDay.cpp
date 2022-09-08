#include "../include/whm/types/HelperDay.hpp"

#include "../include/whm/types/Date.hpp"
#include "../include/whm/types/Day.hpp"

namespace whm {

namespace {
Date calcStartDate(const Date &date)
{
    Q_ASSERT(date.weekday() != Weekday::unknown);

    switch (date.weekday()) {
        case Weekday::monday:
            return date;
        case Weekday::tuesday:
        case Weekday::wednesday:
        case Weekday::thursday:
        case Weekday::friday:
            return date.getPreviouseDateWithWeekday(Weekday::monday);
        case Weekday::saturday:
        case Weekday::sunday:
            return date.getNextDateWithWeekday(Weekday::monday);
        case Weekday::unknown:
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
        case Weekday::monday:
        case Weekday::tuesday:
        case Weekday::wednesday:
        case Weekday::thursday:
        case Weekday::friday: {
            auto day = std::make_shared<Day>(date);
            days.emplace_back(day);
        } break;
        case Weekday::saturday:
            break;
        case Weekday::sunday:
            break;
        case Weekday::unknown:
            break;
    }
}
} // namespace

QVector<std::shared_ptr<Day>>
makeWorkDays(const Date &firstDate, const Date &lastDate)
{
    Q_ASSERT(firstDate < lastDate);

    auto weekday{Weekday::monday};

    auto date = calcStartDate(firstDate);
    auto endDate = lastDate.getNextDateWithWeekday(Weekday::friday);

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
