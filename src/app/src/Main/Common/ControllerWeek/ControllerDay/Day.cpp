
#include "Day.hpp"

namespace whm {

namespace {
bool hasValidTime(const Time &time, bool isHoliday, bool isVacation)
{
    if (isHoliday) {
        return true;
    }
    if (isVacation) {
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
    bool isHoliday,
    bool isVacation)
    : m_date{date}, m_startTime{startTime},
      m_endTime(endTime), m_isHoliday{isHoliday}, m_isVacation{isVacation}
{
}

Day::Day(const Date &date, const Time &startTime, const Time &endTime)
    : Day(date, startTime, endTime, false, false)
{
}

Day::Day(const Date &date) : Day(date, Time{}, Time{}, false, false)
{
}

Date Day::date() const
{
    return m_date;
}

Time Day::startTime() const
{
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

Time Day::endTime() const
{
    return m_endTime;
}

bool Day::setEndTime(const Time &endTime)
{
    if (m_endTime == endTime) {
        return false;
    }
    m_endTime = endTime;
    return true;
}

bool Day::setEndTime(const QString &endTimeAsString)
{
    return m_endTime.set(endTimeAsString);
}

bool Day::isHoliday() const
{
    return m_isHoliday;
}

bool Day::setIsHoliday(bool isHoliday)
{
    if (m_isHoliday == isHoliday) {
        return false;
    }
    m_isHoliday = isHoliday;
    return true;
}

bool Day::isVacation() const
{
    return m_isVacation;
}

bool Day::setisVacation(bool isVacation)
{
    if (m_isVacation == isVacation) {
        return false;
    }
    m_isVacation = isVacation;
    return true;
}

bool Day::hasValidStartTime() const
{
    if (hasValidTime(startTime(), isHoliday(), isVacation())) {
        return true;
    }
    return false;
}

bool Day::hasValidEndTime() const
{
    if (hasValidTime(endTime(), isHoliday(), isVacation())) {
        return true;
    }
    return false;
}

} // namespace whm
