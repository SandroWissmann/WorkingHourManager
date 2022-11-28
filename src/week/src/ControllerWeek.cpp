/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
#include "../include/whm/week/ControllerWeek.hpp"

#include <whm/day/ControllerDay.hpp>
#include <whm/day/Day.hpp>

#include <algorithm>

namespace whm {

namespace {

HoursAndMinutes calculateExpectedWorkedTime(QVector<QObject *> controllerDays);

HoursAndMinutes calculateWorkedTime(const QVector<QObject *> controllerDays);

HoursAndMinutes calculateOvertime(const QVector<QObject *> controllerDays);

Time calculateEarliestEndTime(
    const QVector<QObject *> &controllerDays,
    HoursAndMinutes expectedWorkTime,
    HoursAndMinutes workTime);

} // namespace

ControllerWeek::ControllerWeek(const QVector<QObject *> &controllerDays)
    : m_controllerDays{controllerDays},
      m_expectedWorkTime{calculateExpectedWorkedTime(controllerDays)},
      m_workTime{calculateWorkedTime(m_controllerDays)},
      m_overtime{calculateOvertime(m_controllerDays)}
{
    Q_ASSERT(m_controllerDays.size() == 5);

    for (auto &controllerDay : m_controllerDays) {
        controllerDay->setParent(this);
    }
    makeControllerDaysToThisConnections();

    onWorkTimeOfDayChanged();
    onStartTimeOfDayChanged();
}

QVector<QObject *> ControllerWeek::controllerDays() const
{
    return m_controllerDays;
}

QString ControllerWeek::expectedWorkedTimeAsString() const
{
    if (m_showMinutesAsFractions) {
        return m_expectedWorkTime.toFractionString();
    }
    return m_expectedWorkTime.toString();
}

QString ControllerWeek::workTimeAsString() const
{
    if (m_showMinutesAsFractions) {
        return m_workTime.toFractionString();
    }
    return m_workTime.toString();
}

QString ControllerWeek::overtimeAsString() const
{
    if (m_showMinutesAsFractions) {
        return m_overtime.toFractionString();
    }
    return m_overtime.toString();
}

QString ControllerWeek::earliestEndTimeAsString() const
{
    if (m_showMinutesAsFractions) {
        return m_earliestEndTime.asFractionString();
    }
    return m_earliestEndTime.asString();
}

std::map<int, HoursAndMinutes> ControllerWeek::monthsToWorkTime() const
{
    std::map<int, HoursAndMinutes> monthsToWorkTime;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day->date();
        auto month = date.month();

        if (monthsToWorkTime.find(month) != monthsToWorkTime.end()) {
            auto workTime = monthsToWorkTime[month];
            workTime += HoursAndMinutes{controllerDay->workTime()};
            monthsToWorkTime[month] = workTime;
        }
        else {
            monthsToWorkTime[month] =
                HoursAndMinutes{controllerDay->workTime()};
        }
    }
    return monthsToWorkTime;
}

std::map<int, HoursAndMinutes> ControllerWeek::monthsToOvertime() const
{
    std::map<int, HoursAndMinutes> monthsToOvertime;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day->date();
        auto month = date.month();

        if (monthsToOvertime.find(month) != monthsToOvertime.end()) {
            auto overtime = monthsToOvertime[month];
            overtime += controllerDay->overtime();
            monthsToOvertime[month] = overtime;
        }
        else {
            monthsToOvertime[month] = controllerDay->overtime();
        }
    }
    return monthsToOvertime;
}

std::map<int, QVector<std::shared_ptr<Day>>> ControllerWeek::yearsToDays() const
{
    std::map<int, QVector<std::shared_ptr<Day>>> yearsToDays;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day->date();
        auto year = date.year();

        if (yearsToDays.find(year) == yearsToDays.end()) {
            yearsToDays.insert({year, QVector<std::shared_ptr<Day>>{day}});
        }
        else {
            auto days = yearsToDays.at(year);
            days.emplace_back(day);
            yearsToDays.at(year) = days;
        }
    }
    return yearsToDays;
}

std::map<int, double> ControllerWeek::monthsToUsedFlextimeDays() const
{
    std::map<int, double> monthsToUsedFlextimeDays;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day->date();
        auto month = date.month();

        if (monthsToUsedFlextimeDays.find(month) !=
            monthsToUsedFlextimeDays.end()) {
            auto usedFlextimeDay = monthsToUsedFlextimeDays[month];
            usedFlextimeDay += controllerDay->usedFlextimeDay();
            monthsToUsedFlextimeDays[month] = usedFlextimeDay;
        }
        else {
            monthsToUsedFlextimeDays[month] = controllerDay->usedFlextimeDay();
        }
    }
    return monthsToUsedFlextimeDays;
}

std::map<int, double> ControllerWeek::monthsToUsedVacationDays() const
{
    std::map<int, double> monthsToUsedVacationDays;
    for (const auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        auto day = controllerDay->day();
        auto date = day->date();
        auto month = date.month();

        if (monthsToUsedVacationDays.find(month) !=
            monthsToUsedVacationDays.end()) {
            auto usedVacationDay = monthsToUsedVacationDays[month];
            usedVacationDay += controllerDay->usedVacationDay();
            monthsToUsedVacationDays[month] = usedVacationDay;
        }
        else {
            monthsToUsedVacationDays[month] = controllerDay->usedVacationDay();
        }
    }
    return monthsToUsedVacationDays;
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

void ControllerWeek::setShowMinutesAsFractions(bool showMinutesAsFractions)
{
    if (m_showMinutesAsFractions == showMinutesAsFractions) {
        return;
    }
    m_showMinutesAsFractions = showMinutesAsFractions;

    for (auto &controllerDayAsQObject : m_controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);

        controllerDay->setShowMinutesAsFractions(showMinutesAsFractions);
    }
    emit workTimeChanged();
    emit overtimeChanged();
}

void ControllerWeek::onWorkTimeOfDayChanged()
{
    auto workTime = calculateWorkedTime(m_controllerDays);
    setWorkedTime(workTime);
}

void ControllerWeek::onOvertimeOfDayChanged()
{
    auto overtime = calculateOvertime(m_controllerDays);
    setOvertime(overtime);
}

void ControllerWeek::onStartTimeOfDayChanged()
{
    auto earliestEndTime = calculateEarliestEndTime(
        m_controllerDays, m_expectedWorkTime, m_workTime);
    setEarliestEndTime(earliestEndTime);
}

void ControllerWeek::setWorkedTime(const HoursAndMinutes &workTime)
{
    if (m_workTime == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

void ControllerWeek::setOvertime(const HoursAndMinutes &overtime)
{
    if (m_overtime == overtime) {
        return;
    }
    m_overtime = overtime;
    emit overtimeChanged();
}

void ControllerWeek::setEarliestEndTime(const Time &earliestEndTime)
{
    if (m_earliestEndTime == earliestEndTime) {
        return;
    }
    m_earliestEndTime = earliestEndTime;
    emit earliestEndTimeChanged();
}

void ControllerWeek::makeControllerDaysToThisConnections() const
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
            &ControllerDay::workTimeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::overtimeChanged,
            this,
            &ControllerWeek::onOvertimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::dayTypeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::dayTypeChanged,
            this,
            &ControllerWeek::onOvertimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::defaultWorkTimeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::defaultWorkTimeChanged,
            this,
            &ControllerWeek::onOvertimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::pauseTimeChanged,
            this,
            &ControllerWeek::onWorkTimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::pauseTimeChanged,
            this,
            &ControllerWeek::onOvertimeOfDayChanged);
        connect(
            controllerDay,
            &ControllerDay::usedFlextimeDayChanged,
            this,
            &ControllerWeek::usedFlextimeDaysChanged);
        connect(
            controllerDay,
            &ControllerDay::usedVacationDayChanged,
            this,
            &ControllerWeek::usedVacationDaysChanged);
    }
}

namespace {
Time extractExpectedWorkTime(QVector<QObject *> controllerDays)
{
    Q_ASSERT(!controllerDays.isEmpty());

    auto controllerDay = qobject_cast<ControllerDay *>(controllerDays[0]);
    return controllerDay->defaultWorkTime();
}

HoursAndMinutes calculateExpectedWorkedTime(QVector<QObject *> controllerDays)
{
    HoursAndMinutes expectedWorkedTime;

    for (const auto &controllerDayAsQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayAsQObject);
        HoursAndMinutes expectedWorkedTimeOfDay{
            controllerDay->defaultWorkTime()};

        expectedWorkedTime += expectedWorkedTimeOfDay;
    }
    return expectedWorkedTime;
}

HoursAndMinutes calculateWorkedTime(const QVector<QObject *> controllerDays)
{
    int workMinutes = 0;
    for (const auto &controllerDayQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayQObject);

        auto workTime = controllerDay->workTime();
        workMinutes += workTime.totalMinutes();
    }
    return HoursAndMinutes{workMinutes};
}

HoursAndMinutes calculateOvertime(const QVector<QObject *> controllerDays)
{
    HoursAndMinutes overtimeOfWeek;
    for (const auto &controllerDayQObject : controllerDays) {
        auto controllerDay =
            qobject_cast<ControllerDay *>(controllerDayQObject);

        overtimeOfWeek += controllerDay->overtime();
    }
    return overtimeOfWeek;
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

        if (controllerDay->dayType() != DayType::Work) {
            continue;
        }
        if (controllerDay->hasValidStartTime()) {
            lastDayIt = rit;
            break;
        }
        // last element has not filled out startTime so we cant calc end
        // time
        else {
            return Time{};
        }
    }

    // All days were holiday / vacation or ignore so no calculation
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

    auto remainingWorkTime = expectedWorkTime - workTime;

    auto lastDayController = qobject_cast<ControllerDay *>(*lastDayIt);

    auto startTime = lastDayController->startTime();
    auto pauseTime = lastDayController->pauseTime();

    auto endTime = startTime + remainingWorkTime.toTime() + pauseTime;
    return endTime;
}

} // namespace

} // namespace whm
