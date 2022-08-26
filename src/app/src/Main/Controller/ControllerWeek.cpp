#include "ControllerWeek.hpp"

#include "ControllerDay.hpp"

#include <whm/types/Day.hpp>

#include <algorithm>

namespace whm {

namespace {

HoursAndMinutes calculateExpectedWorkedTime(QVector<QObject *> controllerDays);

HoursAndMinutes calculateWorkedTime(const QVector<QObject *> controllerDays);

Time calculateEarliestEndTime(
    const QVector<QObject *> &controllerDays,
    HoursAndMinutes expectedWorkTime,
    HoursAndMinutes workedTime);

} // namespace

ControllerWeek::ControllerWeek(const QVector<QObject *> &controllerDays)
    : m_controllerDays{controllerDays},
      m_expectedWorkTime{calculateExpectedWorkedTime(controllerDays)},
      m_workedTime{calculateWorkedTime(m_controllerDays)},
      m_overTime{m_workedTime - m_expectedWorkTime}
{
    Q_ASSERT(m_controllerDays.size() == 5);

    for (auto &controllerDay : m_controllerDays) {
        controllerDay->setParent(this);
    }
    makeControllerDayToControllerWeekConnections();

    onWorkTimeOfDayChanged();
    onStartTimeOfDayChanged();
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
        auto date = day->date();
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
        auto date = day->date();
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

QVector<std::shared_ptr<Day>> ControllerWeek::days() const
{
    QVector<std::shared_ptr<Day>> days;
    days.reserve(m_controllerDays.size());
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);
        auto day = controllerDay->day();
        days.emplace_back(day);
    }
    return days;
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
            &ControllerDay::startTimeChanged,
            this,
            &ControllerWeek::onStartTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::workedTimeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::dayTypeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
    }
}

namespace {
Time extractExpectedWorkTime(QVector<QObject *> controllerDays)
{
    Q_ASSERT(!controllerDays.isEmpty());

    auto controllerDay = qobject_cast<ControllerDay *>(controllerDays[0]);
    return controllerDay->defaultWorkedTime();
}

HoursAndMinutes calculateExpectedWorkedTime(QVector<QObject *> controllerDays)
{
    HoursAndMinutes expectedWorkedTime;

    for (const auto &controllerDayAsQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);
        HoursAndMinutes expectedWorkedTimeOfDay{
            controllerDay->defaultWorkedTime()};

        expectedWorkedTime += expectedWorkedTimeOfDay;
    }
    return expectedWorkedTime;
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
    HoursAndMinutes workedTime)
{
    auto lastDayIt = controllerDays.rend();
    for (auto rit = controllerDays.rbegin(); rit != controllerDays.rend();
         ++rit) {

        auto controllerDay = qobject_cast<ControllerDay *>(*rit);

        if (controllerDay->dayType() != DayType::work) {
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

    // All days were holiday / vaccation or ignore so no calculation
    if (lastDayIt == controllerDays.rend()) {
        return Time{};
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

    auto remainingWorkTime = expectedWorkTime - workedTime;

    auto lastDayController = qobject_cast<ControllerDay *>(*lastDayIt);

    auto startTime = lastDayController->startTime();
    auto pauseTime = lastDayController->pauseTime();

    auto endTime = startTime + remainingWorkTime.toTime() + pauseTime;
    return endTime;
}

} // namespace

} // namespace whm
