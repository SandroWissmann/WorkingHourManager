#include "ControllerWeek.hpp"

#include "ControllerWeek/ControllerDay.hpp"

#include <algorithm>

namespace whm {

namespace {

Time toTime(const HoursAndMinutes hoursAndMinutes)
{
    return Time{hoursAndMinutes.hours(), hoursAndMinutes.minutes()};
}

HoursAndMinutes calculateExpectedWorkedTime(Time defaultWorkTimePerDay)
{
    auto minutes =
        defaultWorkTimePerDay.hour() * 60 + defaultWorkTimePerDay.minute();
    minutes *= 5;
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;
    return HoursAndMinutes{hours, minutes};
}

HoursAndMinutes calculateWorkedTime(const QVector<QObject *> controllerDays)
{
    int workedMinutes = 0;
    for (const auto &controllerDayQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayQObject);

        auto workedTime = controllerDay->workedTime();
        workedMinutes += workedTime.totalMinutes();
    }
    return HoursAndMinutes{workedMinutes};
}

Time calculateEarliestEndTime(
    const QVector<QObject *> &controllerDays,
    HoursAndMinutes expectedWorkTime,
    HoursAndMinutes workTime)
{
    auto lastDayIt = controllerDays.rend();
    for (auto rit = controllerDays.rbegin(); rit != controllerDays.rend();
         ++rit) {

        auto controllerDay = qobject_cast<ControllerDay *>(*rit);

        if (controllerDay->isHoliday() || controllerDay->isVacation()) {
            continue;
        }
        if (controllerDay->hasValidStartTime()) {
            lastDayIt = rit;
            break;
        }
        // last element has not filled out startTime so we cant calc end time
        else {
            return Time{};
        }
    }

    for (auto rit = lastDayIt + 1; rit != controllerDays.rend(); ++rit) {
        auto controllerDay = qobject_cast<ControllerDay *>(*rit);

        if (!controllerDay->hasValidStartTime()) {
            return Time{};
        }
        if (!controllerDay->hasValidEndTime()) {
            return Time{};
        }
    }

    auto remainingWorkTime = expectedWorkTime - workTime;

    auto lastDayController = qobject_cast<ControllerDay *>(*lastDayIt);

    auto startTime = lastDayController->startTime();
    auto pauseTime = lastDayController->pauseTime();

    auto endTime = startTime + toTime(remainingWorkTime) + pauseTime;
    return endTime;
}

QVector<QObject *> makeControllerDays(
    const Date &dateOfMonday,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
{
    QVector<QObject *> controllerDays;
    controllerDays.reserve(pauseTimesPerDay.size());

    for (auto i = 0; i < pauseTimesPerDay.size(); ++i) {
        controllerDays.emplaceBack(new ControllerDay{
            Day{dateOfMonday.addDays(i)},
            defaultWorkTimePerDay,
            pauseTimesPerDay[i],
            parent});
    }
    return controllerDays;
}

} // namespace

ControllerWeek::ControllerWeek(
    const Date &dateOfMonday,
    Time defaultWorkTimePerDay,
    const std::array<Time, 5> &pauseTimesPerDay,
    QObject *parent)
    : m_expectedWorkTime{calculateExpectedWorkedTime(defaultWorkTimePerDay)},
      m_overTime{m_workedTime - m_expectedWorkTime},
      m_controllerDays{makeControllerDays(
          dateOfMonday,
          defaultWorkTimePerDay,
          pauseTimesPerDay,
          parent)}
{
    makeControllerDayToControllerWeekConnections();
}

QVector<QObject *> ControllerWeek::controllerDays() const
{
    return m_controllerDays;
}

QString ControllerWeek::expectedWorkedTime() const
{
    return m_expectedWorkTime.toString();
}

QString ControllerWeek::workedTime() const
{
    return m_workedTime.toString();
}

QString ControllerWeek::overTime() const
{
    return m_overTime.toString();
}

QString ControllerWeek::earliestEndTime() const
{
    return m_earliestEndTime.asString();
}

QVector<int> ControllerWeek::months() const
{
    QVector<int> months;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day.date();
        auto month = date.month();

        if (months.isEmpty()) {
            months.emplaceBack(month);
            continue;
        }

        auto it = std::find(months.begin(), months.end(), month);

        if (it != months.end()) {
            continue;
        }
        months.emplaceBack(month);
    }
    return months;
}

// code duplication to months, maybe generalize
QVector<int> ControllerWeek::years() const
{
    QVector<int> years;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day.date();
        auto year = date.year();

        if (years.isEmpty()) {
            years.emplaceBack(year);
            continue;
        }

        auto it = std::find(years.begin(), years.end(), year);

        if (it != years.end()) {
            continue;
        }
        years.emplaceBack(year);
    }
    return years;
}

void ControllerWeek::onWorkTimeOfDayChanged()
{
    auto m_workedTime = calculateWorkedTime(m_controllerDays);

    setWorkedTime(m_workedTime);

    auto overTime = m_workedTime - m_expectedWorkTime;
    setOverTime(overTime);
}

void ControllerWeek::onStartTimeOfDayChanged()
{
    auto earliestEndTime = calculateEarliestEndTime(
        m_controllerDays, m_expectedWorkTime, m_workedTime);
    setEarliestEndTime(earliestEndTime);
}

void ControllerWeek::setWorkedTime(const HoursAndMinutes &workedTime)
{
    if (m_workedTime == workedTime) {
        return;
    }
    m_workedTime = workedTime;
    emit workedTimeChanged();
}

void ControllerWeek::setOverTime(const HoursAndMinutes &overTime)
{
    if (m_overTime == overTime) {
        return;
    }
    m_overTime = overTime;
    emit overTimeChanged();
}

void ControllerWeek::setEarliestEndTime(const Time &earliestEndTime)
{
    if (m_earliestEndTime == earliestEndTime) {
        return;
    }
    m_earliestEndTime = earliestEndTime;
    emit earliestEndTimeChanged();
}

void ControllerWeek::makeControllerDayToControllerWeekConnections() const
{
    for (const auto &controllerDayQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayQObject);
        connect(
            controllerDay,
            &ControllerDay::workTimeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::startTimeChanged,
            this,
            &ControllerWeek::onStartTimeOfDayChanged);
    }
}

} // namespace whm
