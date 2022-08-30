#ifndef WORKING_HOUR_MANAGER_DAY_HPP
#define WORKING_HOUR_MANAGER_DAY_HPP

#include "Date.hpp"
#include "DayType.hpp"
#include "Time.hpp"

namespace whm {

class Day {
public:
    Day(const Date &date,
        const Time &startTime,
        const Time &endTime,
        DayType dayType);

    Day(const Date &date, const Time &startTime, const Time &endTime);

    explicit Day(const Date &date);

    Day(const Day &) = default;
    Day(Day &&) = default;
    Day &operator=(const Day &) = default;
    Day &operator=(Day &&) = default;

    bool isValid() const;

    Date date() const;

    Time startTime() const;
    bool setStartTime(const Time &startTime);
    bool setStartTime(const QString &startTimeAsString);
    bool hasValidStartTime() const;

    Time endTime() const;
    bool setEndTime(Time endTime);
    bool setEndTime(const QString &endTimeAsString);
    bool hasValidEndTime() const;

    DayType dayType() const;
    bool setDayType(DayType dayType);

private:
    Date m_date;
    Time m_startTime;
    Time m_endTime;
    DayType m_dayType{DayType::work};
};
} // namespace whm

#endif
