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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP

#include "DayType.hpp"

#include <whm/types/Date.hpp>
#include <whm/types/HoursAndMinutes.hpp>
#include <whm/types/Time.hpp>

#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

namespace whm {

class Day;
class ControllerDayState;

class ControllerDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString date READ dateAsString CONSTANT)
    Q_PROPERTY(QString weekday READ weekdayAsString CONSTANT)

    Q_PROPERTY(QString startTime READ startTimeAsString WRITE
                   setEnteredStartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTimeAsString WRITE setEnteredEndTime
                   NOTIFY endTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTimeAsString NOTIFY pauseTimeChanged)

    Q_PROPERTY(QString workTime READ workTimeAsString NOTIFY workTimeChanged)
    Q_PROPERTY(QString overtime READ overtimeAsString NOTIFY overtimeChanged)

    Q_PROPERTY(bool timeInputIsEnabled READ timeInputIsEnabled NOTIFY
                   timeInputIsEnabledChanged)

    Q_PROPERTY(
        DayType dayType READ dayType WRITE setDayType NOTIFY dayTypeChanged)
    Q_PROPERTY(
        QVector<QVariant> dayTypesAsVariant READ dayTypesAsVariant CONSTANT)
public:
    ControllerDay(
        std::shared_ptr<Day> day,
        const Time &defaultWorkTime,
        const Time &pauseTime);

    ControllerDay(const ControllerDay &) = delete;
    ControllerDay(ControllerDay &&) = delete;
    ControllerDay &operator=(const ControllerDay &) = delete;
    ControllerDay &operator=(ControllerDay &&) = delete;

    ~ControllerDay() override;

    std::shared_ptr<Day> day() const;

    QString dateAsString() const;

    QString weekdayAsString() const;

    Time defaultWorkTime() const;

    void setEnteredStartTime(const QString &enteredStartTime);
    QString enteredStartTime() const;

    Time startTime() const;
    QString startTimeAsString() const;

    void setEnteredEndTime(QString enteredEndTime);
    QString enteredEndTime();

    Time endTime() const;
    QString endTimeAsString() const;

    Time pauseTime() const;
    QString pauseTimeAsString() const;

    Time workTime() const;
    QString workTimeAsString() const;

    HoursAndMinutes overtime() const;
    QString overtimeAsString() const;

    bool timeInputIsEnabled() const;

    DayType dayType() const;
    void setDayType(DayType dayType);

    QVector<QVariant> dayTypesAsVariant() const;

    bool hasValidStartTime() const;
    bool hasValidEndTime() const;

    double usedFlextimeDay() const;
    double usedVacationDay() const;

    void setState(ControllerDayState *state);

    // TODO These functions should be only used by ControllerDayState and
    // derrived classes but we dont want to use private + friend here because
    // then we need to include all state classes here. Is there a better way?
    void setStartTime(const QString &startTime);
    void setEndTime(const QString &endTime);
    void setWorkTime(const Time &workTime);
    void setOvertime(const HoursAndMinutes &overtime);
    void setTimeInputIsEnabled(bool timeInputIsEnabled);
    void setUsedFlextimeDay(double usedFlextimeDay);
    void setUsedVacationDay(double usedVacationDay);

public slots:
    void onDefaultWorkTimeChanged(const whm::Time &defaultWorkTime);
    void onPauseTimeChanged(const whm::Time &pauseTime);

signals:
    void startTimeChanged();
    void endTimeChanged();
    void pauseTimeChanged();
    void workTimeChanged();
    void defaultWorkTimeChanged();
    void overtimeChanged();
    void timeInputIsEnabledChanged();
    void dayTypeChanged();
    void usedFlextimeDayChanged();
    void usedVacationDayChanged();

private:
    std::shared_ptr<Day> m_day;

    QString m_enteredStartTime{};
    QString m_enteredEndTime{};

    Time m_defaultWorkTime{};
    Time m_pauseTime{};
    Time m_workTime{};

    HoursAndMinutes m_overtime{};

    double m_usedFlextimeDay{};
    double m_usedVacationDay{};

    bool m_timeInputIsEnabled{};

    ControllerDayState *m_state{nullptr};
};

} // namespace whm

#endif
