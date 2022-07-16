#ifndef WORKING_HOUR_MANAGER_DAY_HPP
#define WORKING_HOUR_MANAGER_DAY_HPP

#include "../../../Support/Date.hpp"
#include "../../../Support/Time.hpp"

#include <QDate>
#include <QTime>

namespace whm {

class Day {
public:
    Day(const Date &date,
        const Time &startTime,
        const Time &endTime,
        bool isHoliday,
        bool isVacation);

    Day(const Date &date, const Time &startTime, const Time &endTime);

    explicit Day(const Date &date);

    Day(const Day &) = default;
    Day(Day &&) = default;
    Day &operator=(const Day &) = default;
    Day &operator=(Day &&) = default;

    Date date() const;

    Time startTime() const;
    bool setStartTime(const Time &startTime);
    bool setStartTime(const QString &startTimeAsString);

    Time endTime() const;
    bool setEndTime(const Time &endTime);
    bool setEndTime(const QString &endTimeAsString);

    bool isHoliday() const;
    bool setIsHoliday(bool isHoliday);

    bool isVacation() const;
    bool setisVacation(bool isVacation);

    bool hasValidStartTime() const;
    bool hasValidEndTime() const;

private:
    Date m_date;
    Time m_startTime;
    Time m_endTime;

    bool m_isHoliday{false};
    bool m_isVacation{false};
};
} // namespace whm

#endif
