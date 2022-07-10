#include "Date.hpp"

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

QString Date::weekday() const
{
    return m_date.toString("dddd");
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

Date Date::getPreviouseDateWithDayOfWeek(int dayOfWeek) const
{
    constexpr int mondayIdx = 1;
    constexpr int sundayIdx = 7;
    dayOfWeek = std::clamp(dayOfWeek, mondayIdx, sundayIdx);

    QDate returnDate = m_date;
    while (returnDate.dayOfWeek() != dayOfWeek) {
        returnDate = removeDay(returnDate);
    }
    return {returnDate.year(), returnDate.month(), returnDate.day()};
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
