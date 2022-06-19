#ifndef WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_WEEK_HPP

#include "HoursAndMinutes.hpp"

#include <QDateTime>
#include <QList>
#include <QObject>
#include <QTime>

namespace whm {

class ControllerWeek : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString expectedWorkedTime READ expectedWorkedTime CONSTANT)
    Q_PROPERTY(QString workedTime READ workedTime NOTIFY workedTimeChanged)
    Q_PROPERTY(QString overTime READ overTime NOTIFY overTimeChanged)
    Q_PROPERTY(QString earliestEndTime READ earliestEndTime NOTIFY
                   earliestEndTimeChanged)

    Q_PROPERTY(QList<QObject *> controllerDays READ controllerDays CONSTANT)
public:
    explicit ControllerWeek(QDate dateOfMonday, QTime defaultWorkTimePerDay,
                            QTime pauseTimeMonday, QTime pauseTimeTuesday,
                            QTime pauseTimeWednesday, QTime pauseTimeThursday,
                            QTime pauseTimeFriday, QObject *parent = nullptr);

    ControllerWeek(const ControllerWeek &) = delete;
    ControllerWeek(ControllerWeek &&) = delete;
    ControllerWeek &operator=(const ControllerWeek &) = delete;
    ControllerWeek &operator=(ControllerWeek &&) = delete;

    QList<QObject *> controllerDays() const;

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

    QVector<QObject *> m_controllerDays;
};

} // namespace whm

#endif
