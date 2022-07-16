#ifndef WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP

#include "../Support/Date.hpp"
#include "../Support/Time.hpp"
#include "ControllerWeek/HoursAndMinutes.hpp"

#include <QList>
#include <QObject>

#include <array>

namespace whm {

class Day;

class ControllerWeek : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString expectedWorkedTime READ expectedWorkedTime CONSTANT)
    Q_PROPERTY(QString workedTime READ workedTime NOTIFY workedTimeChanged)
    Q_PROPERTY(QString overTime READ overTime NOTIFY overTimeChanged)
    Q_PROPERTY(QString earliestEndTime READ earliestEndTime NOTIFY
                   earliestEndTimeChanged)

    Q_PROPERTY(QVector<QObject *> controllerDays READ controllerDays CONSTANT)
public:
    ControllerWeek(
        const Date &dateOfMonday,
        const Time &defaultWorkTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        QObject *parent = nullptr);

    ControllerWeek(
        const Time &defaultWorkTimePerDay,
        const std::array<Time, 5> &pauseTimesPerDay,
        const std::array<Day, 5> &days);

    ControllerWeek(const ControllerWeek &) = delete;
    ControllerWeek(ControllerWeek &&) = delete;
    ControllerWeek &operator=(const ControllerWeek &) = delete;
    ControllerWeek &operator=(ControllerWeek &&) = delete;

    ~ControllerWeek() = default;

    QVector<QObject *> controllerDays() const;

    QString expectedWorkedTime() const;
    QString workedTime() const;
    QString overTime() const;
    QString earliestEndTime() const;

    // Indicate in which month the week is present
    QVector<int> months() const;

    // Indicate in which year the week is present
    QVector<int> years() const;

signals:
    void workedTimeChanged();
    void overTimeChanged();
    void earliestEndTimeChanged();

private slots:
    void onWorkTimeOfDayChanged();
    void onStartTimeOfDayChanged();

private:
    void setWorkedTime(const HoursAndMinutes &workedTime);
    void setOverTime(const HoursAndMinutes &overTime);
    void setEarliestEndTime(const Time &earliestEndTime);
    void makeControllerDayToControllerWeekConnections() const;

    HoursAndMinutes m_expectedWorkTime;
    HoursAndMinutes m_workedTime{};
    HoursAndMinutes m_overTime;
    Time m_earliestEndTime{};

    QVector<QObject *> m_controllerDays;
};

} // namespace whm

#endif
