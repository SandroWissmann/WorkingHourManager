#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP

#include "../../Support/Date.hpp"
#include "../../Support/Time.hpp"
#include "ControllerDay/Day.hpp"

#include <QObject>

namespace whm {

class ControllerDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString date READ dateAsString CONSTANT)
    Q_PROPERTY(QString weekday READ weekday CONSTANT)

    Q_PROPERTY(QString startTime READ startTimeAsString WRITE setStartTime
                   NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTimeAsString WRITE setEndTime NOTIFY
                   endTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTimeAsString CONSTANT)

    Q_PROPERTY(QString workTime READ workTimeAsString NOTIFY workTimeChanged)

    Q_PROPERTY(bool isHoliday READ isHoliday WRITE setIsHoliday NOTIFY
                   isHolidayChanged)

    Q_PROPERTY(bool isVacation READ isVacation WRITE setisVacation NOTIFY
                   isVacationChanged)
public:
    ControllerDay(
        const Day &day,
        const Time &defaultWorkTime,
        const Time &pauseTime,
        QObject *parent = nullptr);

    ControllerDay(const ControllerDay &) = delete;
    ControllerDay(ControllerDay &&) = delete;
    ControllerDay &operator=(const ControllerDay &) = delete;
    ControllerDay &operator=(ControllerDay &&) = delete;

    ~ControllerDay() = default;

    Day day() const;

    QString dateAsString() const;

    QString weekday() const;

    Time startTime() const;
    QString startTimeAsString() const;
    void setStartTime(const QString &startTime);

    QString endTimeAsString() const;
    void setEndTime(const QString &endTime);

    Time pauseTime() const;
    QString pauseTimeAsString() const;

    Time workedTime() const;
    QString workTimeAsString() const;

    bool isHoliday() const;
    void setIsHoliday(bool isHoliday);

    bool isVacation() const;
    void setisVacation(bool isVacation);

    bool hasValidStartTime() const;
    bool hasValidEndTime() const;

signals:
    void startTimeChanged();
    void endTimeChanged();
    void workTimeChanged();
    void isHolidayChanged();
    void isVacationChanged();

private:
    void calcWorkTime();
    void setWorkTime(const Time &workTime);

    Day m_day;

    Time m_defaultWorkTime;
    Time m_pauseTime;
    Time m_workTime{};
};

} // namespace whm

#endif
