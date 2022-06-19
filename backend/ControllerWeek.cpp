#include "ControllerWeek.hpp"

#include "ControllerDay.hpp"

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

HoursAndMinutes
calculateWorkedTime(const QVector<ControllerDay *> controllerDays)
{
    int workedMinutes{0};
    for (const auto &controllerDay : controllerDays) {
        workedMinutes += controllerDay->workedMinutes();
    }

    auto workedHours = workedMinutes / 60;
    workedMinutes = workedMinutes - workedHours * 60;
    return HoursAndMinutes{workedHours, workedMinutes};
}

HoursAndMinutes
calculateEarliestEndTime(const QVector<ControllerDay *> &controllerDays,
                         HoursAndMinutes expectedWorkTime,
                         HoursAndMinutes workTime)
{
    auto lastDayIt = controllerDays.rend();
    for (auto rit = controllerDays.rbegin(); rit != controllerDays.rend();
         ++rit) {

        if ((*rit)->isHoliday() || (*rit)->isVaccation()) {
            continue;
        }
        if ((*rit)->hasValidStartTime()) {
            lastDayIt = rit;
            break;
        }
        // last element has not filled out startTime so we cant calc end time
        else {
            return HoursAndMinutes{};
        }
    }

    for (auto rit = lastDayIt + 1; rit != controllerDays.rend(); ++rit) {
        if (!(*rit)->hasValidStartTime()) {
            return HoursAndMinutes{};
        }
        if (!(*rit)->hasValidEndTime()) {
            return HoursAndMinutes{};
        }
    }

    auto remainingWorkTime = expectedWorkTime - workTime;
    auto startTime = qTimeToHoursAndMinutes((*lastDayIt)->startTimeAsTime());
    auto pauseTime = qTimeToHoursAndMinutes((*lastDayIt)->pauseTimeAsTime());
    auto endTime = startTime + remainingWorkTime + pauseTime;
    return endTime;
}

QVector<ControllerDay *>
makeControllerDays(QDate dateOfMonday, QTime defaultWorkTimePerDay,
                   QTime pauseTimeMonday, QTime pauseTimeTuesday,
                   QTime pauseTimeWednesday, QTime pauseTimeThursday,
                   QTime pauseTimeFriday, QObject *parent)
{
    return {new ControllerDay{dateOfMonday, defaultWorkTimePerDay,
                              pauseTimeMonday, parent},
            new ControllerDay{dateOfMonday.addDays(1), defaultWorkTimePerDay,
                              pauseTimeTuesday, parent},
            new ControllerDay{dateOfMonday.addDays(2), defaultWorkTimePerDay,
                              pauseTimeWednesday, parent},
            new ControllerDay{dateOfMonday.addDays(3), defaultWorkTimePerDay,
                              pauseTimeThursday, parent},
            new ControllerDay{dateOfMonday.addDays(4), defaultWorkTimePerDay,
                              pauseTimeFriday, parent}};
}

} // namespace

ControllerWeek::ControllerWeek(QDate dateOfMonday, QTime defaultWorkTimePerDay,
                               QTime pauseTimeMonday, QTime pauseTimeTuesday,
                               QTime pauseTimeWednesday,
                               QTime pauseTimeThursday, QTime pauseTimeFriday,
                               QObject *parent)
    : m_expectedWorkTime{calculateExpectedWorkedTime(defaultWorkTimePerDay)},
      m_overTime{m_workedTime - m_expectedWorkTime},
      m_controllerDays{makeControllerDays(dateOfMonday, defaultWorkTimePerDay,
                                          pauseTimeMonday, pauseTimeTuesday,
                                          pauseTimeWednesday, pauseTimeThursday,
                                          pauseTimeFriday, parent)}
{
    makeControllerDayToControllerWeekConnections();
}

QObject *ControllerWeek::controllerMonday() const
{
    return m_controllerDays[0];
}

QObject *ControllerWeek::controllerTuesday() const
{
    return m_controllerDays[1];
}

QObject *ControllerWeek::controllerWednesday() const
{
    return m_controllerDays[2];
}

QObject *ControllerWeek::controllerThursday() const
{
    return m_controllerDays[3];
}

QObject *ControllerWeek::controllerFriday() const
{
    return m_controllerDays[4];
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
    for (const auto &controller : m_controllerDays) {
        connect(controller, &ControllerDay::workTimeChanged, this,
                &ControllerWeek::onWorkTimeOfDayChanged);
        connect(controller, &ControllerDay::startTimeChanged, this,
                &ControllerWeek::onStartTimeOfDayChanged);
    }
}

} // namespace whm
