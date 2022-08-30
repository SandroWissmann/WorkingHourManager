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
    Q_PROPERTY(QString expectedWorkedTime READ expectedWorkedTime CONSTANT)
    Q_PROPERTY(QString workTime READ workTime NOTIFY workTimeChanged)
    Q_PROPERTY(QString overtime READ overtime NOTIFY overtimeChanged)
    Q_PROPERTY(QString earliestEndTime READ earliestEndTime NOTIFY
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

    QString expectedWorkedTime() const;
    QString workTime() const;
    QString overtime() const;
    QString earliestEndTime() const;

    // If week is in two months we can access here the split overtime for the
    // days in each month
    QMap<int, HoursAndMinutes> monthsToOvertime() const;

    // Indicate in which month the week is present
    QVector<int> months() const;

    // Indicate in which year the week is present
    QVector<int> years() const;

    QVector<std::shared_ptr<Day>> days() const;

signals:
    void workTimeChanged();
    void overtimeChanged();
    void earliestEndTimeChanged();

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
};

} // namespace whm

#endif
