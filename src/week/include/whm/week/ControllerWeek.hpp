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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP

#include <whm/types/Date.hpp>
#include <whm/types/HoursAndMinutes.hpp>
#include <whm/types/Time.hpp>

#include <QList>
#include <QObject>

#include <array>
#include <memory>

namespace whm {

class Day;

class ControllerWeek : public QObject {
    Q_OBJECT
    Q_PROPERTY(
        QString expectedWorkedTime READ expectedWorkedTimeAsString CONSTANT)
    Q_PROPERTY(QString workTime READ workTimeAsString NOTIFY workTimeChanged)
    Q_PROPERTY(QString overtime READ overtimeAsString NOTIFY overtimeChanged)
    Q_PROPERTY(QString earliestEndTime READ earliestEndTimeAsString NOTIFY
                   earliestEndTimeChanged)

    Q_PROPERTY(QVector<QObject *> controllerDays READ controllerDays CONSTANT)
public:
    ControllerWeek(const QVector<QObject *> &controllerDays);

    ControllerWeek(const ControllerWeek &) = delete;
    ControllerWeek(ControllerWeek &&) = delete;
    ControllerWeek &operator=(const ControllerWeek &) = delete;
    ControllerWeek &operator=(ControllerWeek &&) = delete;

    ~ControllerWeek() = default;

    QVector<QObject *> controllerDays() const;

    QString expectedWorkedTimeAsString() const;
    QString workTimeAsString() const;
    QString overtimeAsString() const;
    QString earliestEndTimeAsString() const;

    // If week is in two months we can access here the split worktime for the
    // days in each month
    std::map<int, HoursAndMinutes> monthsToWorkTime() const;

    // If week is in two months we can access here the split overtime for the
    // days in each month
    std::map<int, HoursAndMinutes> monthsToOvertime() const;

    std::map<int, QVector<std::shared_ptr<Day>>> yearsToDays() const;

    std::map<int, double> monthsToUsedFlextimeDays() const;

    std::map<int, double> monthsToUsedVacationDays() const;

    // Indicate in which month the week is present
    QVector<int> months() const;

    // Indicate in which year the week is present
    QVector<int> years() const;

    QVector<std::shared_ptr<Day>> days() const;

    void setShowMinutesAsFractions(bool showMinutesAsFractions);

signals:
    void workTimeChanged();
    void overtimeChanged();
    void earliestEndTimeChanged();
    void usedFlextimeDaysChanged();
    void usedVacationDaysChanged();

private slots:
    void onWorkTimeOfDayChanged();
    void onOvertimeOfDayChanged();
    void onStartTimeOfDayChanged();

private:
    void setWorkedTime(const HoursAndMinutes &workTime);
    void setOvertime(const HoursAndMinutes &overtime);
    void setEarliestEndTime(const Time &earliestEndTime);
    void makeControllerDaysToThisConnections() const;

    QVector<QObject *> m_controllerDays;

    HoursAndMinutes m_expectedWorkTime;
    HoursAndMinutes m_workTime{};
    HoursAndMinutes m_overtime;
    Time m_earliestEndTime{};

    bool m_showMinutesAsFractions{false};
};

} // namespace whm

#endif
