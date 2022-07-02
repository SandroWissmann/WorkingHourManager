#include "ControllerWeek.hpp"

#include "ControllerWeek/ControllerDay.hpp"

#include <algorithm>

namespace whm {

namespace {

HoursAndMinutes qTimeToHoursAndMinutes(const QTime &time)
{
    // note: slicing happens here we only keep hours and minutes
    return HoursAndMinutes{time.hour(), time.minute()};
}

HoursAndMinutes calculateExpectedWorkedTime(QTime defaultWorkTimePerDay)
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
    int workedMinutes{0};
    for (const auto &controllerDayQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayQObject);
        workedMinutes += controllerDay->workedMinutes();
    }

    auto workedHours = workedMinutes / 60;
    workedMinutes = workedMinutes - workedHours * 60;
    return HoursAndMinutes{workedHours, workedMinutes};
}

HoursAndMinutes calculateEarliestEndTime(
    const QVector<QObject *> &controllerDays,
    HoursAndMinutes expectedWorkTime,
    HoursAndMinutes workTime)
{
    auto lastDayIt = controllerDays.rend();
    for (auto rit = controllerDays.rbegin(); rit != controllerDays.rend();
         ++rit) {

        auto controllerDay = qobject_cast<ControllerDay *>(*rit);

        if (controllerDay->isHoliday() || controllerDay->isVaccation()) {
            continue;
        }
        if (controllerDay->hasValidStartTime()) {
            lastDayIt = rit;
            break;
        }
        // last element has not filled out startTime so we cant calc end time
        else {
            return HoursAndMinutes{};
        }
    }

    for (auto rit = lastDayIt + 1; rit != controllerDays.rend(); ++rit) {

        auto controllerDay = qobject_cast<ControllerDay *>(*rit);

        if (!controllerDay->hasValidStartTime()) {
            return HoursAndMinutes{};
        }
        if (!controllerDay->hasValidEndTime()) {
            return HoursAndMinutes{};
        }
    }

    auto remainingWorkTime = expectedWorkTime - workTime;

    auto lastDayController = qobject_cast<ControllerDay *>(*lastDayIt);

    auto startTime =
        qTimeToHoursAndMinutes(lastDayController->startTimeAsTime());
    auto pauseTime =
        qTimeToHoursAndMinutes(lastDayController->pauseTimeAsTime());
    auto endTime = startTime + remainingWorkTime + pauseTime;
    return endTime;
}

QVector<QObject *> makeControllerDays(
    const QDate &dateOfMonday,
    QTime defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
    QObject *parent)
{
    QVector<QObject *> controllerDays;
    controllerDays.reserve(pauseTimesPerDay.size());

    for (auto i = 0; i < pauseTimesPerDay.size(); ++i) {
        controllerDays.emplaceBack(new ControllerDay{
            dateOfMonday.addDays(i),
            defaultWorkTimePerDay,
            pauseTimesPerDay[i],
            parent});
    }
    return controllerDays;
}

} // namespace

ControllerWeek::ControllerWeek(
    const QDate &dateOfMonday,
    QTime defaultWorkTimePerDay,
    const std::array<QTime, 5> &pauseTimesPerDay,
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
    return m_earliestEndTime.toString();
}

QVector<int> ControllerWeek::months() const
{
    QVector<int> months;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto month = controllerDay->month();

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

        auto year = controllerDay->year();

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

void ControllerWeek::setEarliestEndTime(const HoursAndMinutes &earliestEndTime)
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
