#include "../include/whm/types/Day.hpp"

namespace whm {

namespace {
bool hasValidTime(
    const Time &time,
    bool isHoliday,
    bool isVacation,
    bool isIgnore)
{
    if (isHoliday) {
        return true;
    }
    if (isVacation) {
        return true;
    }
    if (isIgnore) {
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
    bool isVacation,
    bool isIgnore)
    : m_date{date}, m_startTime{startTime}, m_endTime(endTime),
      m_isHoliday{isHoliday}, m_isVacation{isVacation}, m_isIgnore{isIgnore}
{
}

Day::Day(const Date &date, const Time &startTime, const Time &endTime)
    : Day(date, startTime, endTime, false, false, false)
{
}

Day::Day(const Date &date) : Day(date, Time{}, Time{}, false, false, false)
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

bool Day::setEndTime(Time endTime)
{
    // detect input as PM and transform to to 24h format.
    if (endTime.isValid() && endTime < m_startTime && endTime.hour() < 12) {
        auto hour = endTime.hour();
        auto minute = endTime.minute();
        hour += 12;
        endTime = Time{hour, minute};
    }

    if (m_endTime == endTime) {
        return false;
    }
    m_endTime = endTime;
    return true;
}

bool Day::setEndTime(const QString &endTimeAsString)
{
    Time endTime;
    endTime.set(endTimeAsString);
    return setEndTime(endTime);
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

bool Day::setIsVacation(bool isVacation)
{
    if (m_isVacation == isVacation) {
        return false;
    }
    m_isVacation = isVacation;
    return true;
}

bool Day::isIgnore() const
{
    return m_isIgnore;
}

bool Day::setIsIgnore(bool isIgnore)
{
    if (m_isIgnore == isIgnore) {
        return false;
    }
    m_isIgnore = isIgnore;
    return true;
}

bool Day::hasValidStartTime() const
{
    if (hasValidTime(startTime(), isHoliday(), isVacation(), isIgnore())) {
        return true;
    }
    return false;
}

bool Day::hasValidEndTime() const
{
    if (hasValidTime(endTime(), isHoliday(), isVacation(), isIgnore())) {
        return true;
    }
    return false;
}

} // namespace whm
