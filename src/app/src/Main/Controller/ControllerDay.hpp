#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP

#include <whm/types/Date.hpp>
#include <whm/types/DayType.hpp>
#include <whm/types/Time.hpp>

#include <QObject>

#include <memory>

namespace whm {

class Day;

class ControllerDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString date READ dateAsString CONSTANT)
    Q_PROPERTY(QString weekday READ weekday CONSTANT)

    Q_PROPERTY(QString startTime READ startTimeAsString WRITE setStartTime
                   NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTimeAsString WRITE setEndTime NOTIFY
                   endTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTimeAsString CONSTANT)

    Q_PROPERTY(QString workedTime READ workedTimeAsString NOTIFY workedTimeChanged)

    Q_PROPERTY(
        DayType dayType READ dayType WRITE setDayType NOTIFY dayTypeChanged)
public:
    ControllerDay(
        std::shared_ptr<Day> day,
        const Time &defaultWorkedTime,
        const Time &pauseTime);

    ControllerDay(const ControllerDay &) = delete;
    ControllerDay(ControllerDay &&) = delete;
    ControllerDay &operator=(const ControllerDay &) = delete;
    ControllerDay &operator=(ControllerDay &&) = delete;

    ~ControllerDay() override;

    std::shared_ptr<Day> day() const;

    QString dateAsString() const;

    QString weekday() const;

    Time defaultWorkedTime() const;

    Time startTime() const;
    QString startTimeAsString() const;
    void setStartTime(const QString &startTime);

    QString endTimeAsString() const;
    void setEndTime(const QString &endTime);

    Time pauseTime() const;
    QString pauseTimeAsString() const;

    Time workedTime() const;
    QString workedTimeAsString() const;

    DayType dayType() const;
    void setDayType(DayType dayType);

    bool hasValidStartTime() const;
    bool hasValidEndTime() const;

signals:
    void startTimeChanged();
    void endTimeChanged();
    void workedTimeChanged();
    void dayTypeChanged();

private:
    void setWorkTime(const Time &workedTime);

    std::shared_ptr<Day> m_day;

    Time m_defaultWorkedTime;
    Time m_pauseTime;
    Time m_workedTime{};
};

} // namespace whm

#endif
