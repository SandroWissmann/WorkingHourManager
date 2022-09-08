#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP

#include <whm/types/Date.hpp>
#include <whm/types/DayType.hpp>
#include <whm/types/HoursAndMinutes.hpp>
#include <whm/types/Time.hpp>

#include <QObject>
#include <QString>
#include <QStringList>

#include <memory>

namespace whm {

class Day;

class ControllerDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString date READ dateAsString CONSTANT)
    Q_PROPERTY(QString weekday READ weekdayAsString CONSTANT)

    Q_PROPERTY(QString startTime READ startTimeAsString WRITE setStartTime
                   NOTIFY startTimeChanged)
    Q_PROPERTY(QString endTime READ endTimeAsString WRITE setEndTime NOTIFY
                   endTimeChanged)

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

    Time startTime() const;
    QString startTimeAsString() const;
    void setStartTime(const QString &startTime);

    QString endTimeAsString() const;
    void setEndTime(const QString &endTime);

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
    void setPauseTime(const Time &pauseTime);
    void setWorkTime(const Time &workTime);
    void setDefaultWorkTime(const Time &defaultWorkTime);

    void setUsedFlextimeDay(double usedFlextimeDay);
    void setUsedVacationDay(double usedVacationDay);

    std::shared_ptr<Day> m_day;

    Time m_defaultWorkTime;
    Time m_pauseTime;
    Time m_workTime{};

    double m_usedFlextimeDay;
    double m_usedVacationDay;
};

} // namespace whm

#endif
