#include "../include/whm/types/Day.hpp"

namespace whm {

namespace {
bool hasValidTime(const Time &time, DayType dayType)
{
    if (dayType != DayType::work) {
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
    DayType dayType)
    : m_date{date}, m_startTime{startTime},
      m_endTime(endTime), m_dayType{dayType}
{
}

Day::Day(const Date &date, const Time &startTime, const Time &endTime)
    : Day(date, startTime, endTime, DayType::work)
{
}

Day::Day(const Date &date) : Day(date, Time{}, Time{}, DayType::work)
{
}

Date Day::date() const
{
    return m_date;
}

Time Day::startTime() const
{
    if (m_dayType != DayType::work) {
        return Time{};
    }
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

bool Day::hasValidStartTime() const
{
    if (hasValidTime(startTime(), dayType())) {
        return true;
    }
    return false;
}

Time Day::endTime() const
{
    if (m_dayType != DayType::work) {
        return Time{};
    }
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

bool Day::hasValidEndTime() const
{
    if (hasValidTime(endTime(), dayType())) {
        return true;
    }
    return false;
}

DayType Day::dayType() const
{
    return m_dayType;
}

bool Day::setDayType(DayType dayType)
{
    if (m_dayType == dayType) {
        return false;
    }
    m_dayType = dayType;
    return true;
}

} // namespace whm
