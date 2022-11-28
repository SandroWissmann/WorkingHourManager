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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_MONTH_HPP

#include <QDate>
#include <QObject>
#include <QVector>

#include <whm/types/HoursAndMinutes.hpp>

namespace whm {

class Day;

class ControllerMonth : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerWeeks READ controllerWeeks CONSTANT)
    Q_PROPERTY(QString nameOfMonth READ nameOfMonth CONSTANT)
    Q_PROPERTY(QString workTime READ workTimeAsString NOTIFY workTimeChanged)
    Q_PROPERTY(QString overtime READ overtimeAsString NOTIFY overtimeChanged)
public:
    ControllerMonth(const QVector<QObject *> &controllerWeeks);

    ControllerMonth(const ControllerMonth &) = delete;
    ControllerMonth(ControllerMonth &&) = delete;
    ControllerMonth &operator=(const ControllerMonth &) = delete;
    ControllerMonth &operator=(ControllerMonth &&) = delete;

    ~ControllerMonth() = default;

    QVector<QObject *> controllerWeeks() const;

    QVector<QObject *> controllerDays() const;

    QString nameOfMonth() const;

    HoursAndMinutes workTime() const;
    QString workTimeAsString() const;

    HoursAndMinutes overtime() const;
    QString overtimeAsString() const;

    double usedFlextimeDays() const;
    double usedVacationDays() const;

    QVector<std::shared_ptr<Day>> days() const;

    int month() const;
    int year() const;

    void setShowMinutesAsFractions(bool showMinutesAsFractions);

signals:
    void workTimeChanged();
    void overtimeChanged();
    void usedFlextimeDaysChanged();
    void usedVacationDaysChanged();

private slots:
    void onWorkTimeOfWeekChanged();
    void onOvertimeOfWeekChanged();
    void onUsedFlextimeDaysOfWeekChanged();
    void onUsedVacationDaysOfWeekChanged();

private:
    void makeControllerWeeksToThisConnections() const;

    void setWorkTime(const HoursAndMinutes &workTime);
    void setOvertime(const HoursAndMinutes &overtime);
    void setUsedFlextimeDays(double usedFlextimeDays);
    void setUsedVacationDays(double usedVacationDays);

    QVector<QObject *> m_controllerWeeks;

    HoursAndMinutes m_workTime;
    HoursAndMinutes m_overtime;
    double m_usedFlextimeDays;
    double m_usedVacationDays;

    bool m_showMinutesAsFractions{false};
};
} // namespace whm

#endif
