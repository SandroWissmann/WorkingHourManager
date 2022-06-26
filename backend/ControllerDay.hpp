#ifndef WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_DAY_HPP

#include <QDate>
#include <QObject>
#include <QTime>

namespace whm {

class ControllerDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString date READ date CONSTANT)
    Q_PROPERTY(QString day READ day CONSTANT)

    Q_PROPERTY(QString startTime READ startTime WRITE setStartTime NOTIFY
                   startTimeChanged)
    Q_PROPERTY(
        QString endTime READ endTime WRITE setEndTime NOTIFY endTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTime CONSTANT)

    Q_PROPERTY(QString workTime READ workTime NOTIFY workTimeChanged)

    Q_PROPERTY(bool isHoliday READ isHoliday WRITE setIsHoliday NOTIFY
                   isHolidayChanged)

    Q_PROPERTY(bool isVaccation READ isVaccation WRITE setIsVaccation NOTIFY
                   isVaccationChanged)
public:
    explicit ControllerDay(QDate date, QTime defaultWorkTime, QTime pauseTime,
                           QObject *parent = nullptr);

    ControllerDay(const ControllerDay &) = delete;
    ControllerDay(ControllerDay &&) = delete;
    ControllerDay &operator=(const ControllerDay &) = delete;
    ControllerDay &operator=(ControllerDay &&) = delete;

    ~ControllerDay() = default;

    // format dd.MM.yyyy
    QString date() const;
    // Monday / Tuesday etc.
    QString day() const;

    int month() const;

    int year() const;

    QString startTime() const;
    QTime startTimeAsTime() const;
    void setStartTime(const QString &startTime);

    QString endTime() const;
    void setEndTime(const QString &endTime);

    QString pauseTime() const;
    QTime pauseTimeAsTime() const;

    QString workTime() const;
    int workedMinutes() const;

    bool isHoliday() const;
    void setIsHoliday(bool isHoliday);

    bool isVaccation() const;
    void setIsVaccation(bool isVaccation);

    bool hasValidStartTime() const;
    bool hasValidEndTime() const;

signals:
    void startTimeChanged();
    void endTimeChanged();
    void workTimeChanged();
    void isHolidayChanged();
    void isVaccationChanged();

private:
    void setStartTime(const QTime &startTime);
    void setEndTime(const QTime &endTime);

    void calcWorkTime();
    void setWorkTime(const QTime &workTime);

    QDate m_date;

    QTime m_startTime;
    QTime m_endTime;
    QTime m_defaultWorkTime;
    QTime m_pauseTime;
    QTime m_workTime{};

    bool m_isHoliday{false};
    bool m_isVaccation{false};
};

} // namespace whm

#endif
