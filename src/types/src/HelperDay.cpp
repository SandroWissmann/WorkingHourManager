#include "../include/whm/types/HelperDay.hpp"

#include "../include/whm/types/Date.hpp"
#include "../include/whm/types/Day.hpp"

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
