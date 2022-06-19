#ifndef WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP

#include "HoursAndMinutes.hpp"

#include <QDateTime>
#include <QObject>
#include <QTime>
#include <QVector>

namespace whm {

class ControllerDay;

class ControllerWeek : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString expectedWorkedTime READ expectedWorkedTime CONSTANT)
    Q_PROPERTY(QString workedTime READ workedTime NOTIFY workedTimeChanged)
    Q_PROPERTY(QString overTime READ overTime NOTIFY overTimeChanged)
    Q_PROPERTY(QString earliestEndTime READ earliestEndTime NOTIFY
                   earliestEndTimeChanged)

    Q_PROPERTY(QObject *controllerMonday READ controllerMonday CONSTANT)
    Q_PROPERTY(QObject *controllerTuesday READ controllerTuesday CONSTANT)
    Q_PROPERTY(QObject *controllerWednesday READ controllerWednesday CONSTANT)
    Q_PROPERTY(QObject *controllerThursday READ controllerThursday CONSTANT)
    Q_PROPERTY(QObject *controllerFriday READ controllerFriday CONSTANT)
public:
    explicit ControllerWeek(QDate dateOfMonday, QTime defaultWorkTimePerDay,
                            QTime pauseTimeMonday, QTime pauseTimeTuesday,
                            QTime pauseTimeWednesday, QTime pauseTimeThursday,
                            QTime pauseTimeFriday, QObject *parent = nullptr);

    ControllerWeek(const ControllerWeek &) = delete;
    ControllerWeek(ControllerWeek &&) = delete;
    ControllerWeek &operator=(const ControllerWeek &) = delete;
    ControllerWeek &operator=(ControllerWeek &&) = delete;

    QObject *controllerMonday() const;
    QObject *controllerTuesday() const;
    QObject *controllerWednesday() const;
    QObject *controllerThursday() const;
    QObject *controllerFriday() const;

    QString expectedWorkedTime() const;
    QString workedTime() const;
    QString overTime() const;
    QString earliestEndTime() const;

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
    void setEarliestEndTime(const HoursAndMinutes &earliestEndTime);
    void makeControllerDayToControllerWeekConnections() const;

    HoursAndMinutes m_expectedWorkTime;
    HoursAndMinutes m_workedTime{};
    HoursAndMinutes m_overTime;
    HoursAndMinutes m_earliestEndTime{};

    QVector<ControllerDay *> m_controllerDays;
};

} // namespace whm

#endif
