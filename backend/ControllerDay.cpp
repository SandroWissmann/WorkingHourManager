#include "ControllerDay.hpp"

#include <QDebug>

namespace whm {

namespace {

QTime makeInitTime()
{
    return QTime::fromString("00:00", "hh:mm");
}

QTime stringToTime(const QString hhmmStr)
{
    return QTime::fromString(hhmmStr, "hh:mm");
}

QString timeToString(const QTime &time)
{
    return time.toString("hh:mm");
}

int timeToMinutes(const QTime &time)
{
    auto hour = time.hour();
    auto minutes = time.minute();
    return hour * 60 + minutes;
}

QTime minutesToTime(int minutes)
{
    auto hours = minutes / 60;
    minutes = minutes - hours * 60;

    return QTime{hours, minutes};
}

QTime workTimeFromTimes(QTime startTime, QTime endTime, QTime pauseTime)
{
    auto startMinutes = timeToMinutes(startTime);
    auto endMinutes = timeToMinutes(endTime);
    auto pauseMinutes = timeToMinutes(pauseTime);

    auto workedMinutes = endMinutes - startMinutes - pauseMinutes;
    auto workTime = minutesToTime(workedMinutes);
    return workTime;
}

bool setTime(QTime &time, const QTime &newTime)
{
    if (time == newTime) {
        return false;
    }
    time = newTime;
    if (!time.isValid()) {
        time = makeInitTime();
    }
    return true;
}

bool hasValidTime(const QTime &time, bool isHoliday, bool isVaccation)
{
    if (isHoliday) {
        return true;
    }
    if (isVaccation) {
        return true;
    }
    if (timeToString(time) == "00:00") {
        return false;
    }
    return true;
}

} // namespace

ControllerDay::ControllerDay(QDate date, QTime defaultWorkTime, QTime pauseTime,
                             QObject *parent)
    : QObject{parent}, m_date{date},
      m_startTime{makeInitTime()}, m_endTime{makeInitTime()},
      m_defaultWorkTime{defaultWorkTime}, m_pauseTime{pauseTime}
{
    if (!m_pauseTime.isValid()) {
        qDebug() << Q_FUNC_INFO << "Invalid pause time passed";
        m_pauseTime = makeInitTime();
    }
}

QString ControllerDay::date() const
{
    return m_date.toString("dd.MM.yyyy");
}

QString ControllerDay::day() const
{
    return m_date.toString("dddd");
}

int ControllerDay::month() const
{
    return m_date.month();
}

int ControllerDay::year() const
{
    return m_date.year();
}

QString ControllerDay::startTime() const
{
    return timeToString(m_startTime);
}

QTime ControllerDay::startTimeAsTime() const
{
    return m_startTime;
}

void ControllerDay::setStartTime(const QString &startTime)
{
    auto newTime = stringToTime(startTime);
    setStartTime(newTime);
}

QString ControllerDay::endTime() const
{
    return timeToString(m_endTime);
}

void ControllerDay::setEndTime(const QString &endTime)
{
    auto newTime = stringToTime(endTime);
    setEndTime(newTime);
}

QString ControllerDay::pauseTime() const
{
    return timeToString(m_pauseTime);
}

QTime ControllerDay::pauseTimeAsTime() const
{
    return m_pauseTime;
}

QString ControllerDay::workTime() const
{
    return timeToString(m_workTime);
}

int ControllerDay::workedMinutes() const
{
    if (!m_workTime.isValid()) {
        return 0;
    }
    return m_workTime.hour() * 60 + m_workTime.minute();
}

bool ControllerDay::isHoliday() const
{
    return m_isHoliday;
}

void ControllerDay::setIsHoliday(bool isHoliday)
{
    if (m_isHoliday == isHoliday) {
        return;
    }
    m_isHoliday = isHoliday;
    emit isHolidayChanged();
    setStartTime(makeInitTime());
    setEndTime(makeInitTime());
    calcWorkTime();
}

bool ControllerDay::isVaccation() const
{
    return m_isVaccation;
}

bool ControllerDay::hasValidStartTime() const
{
    if (hasValidTime(m_startTime, isHoliday(), isVaccation())) {
        return true;
    }
    return false;
}

bool ControllerDay::hasValidEndTime() const
{
    if (hasValidTime(m_endTime, isHoliday(), isVaccation())) {
        return true;
    }
    return false;
}

void ControllerDay::setIsVaccation(bool isVaccation)
{
    if (m_isVaccation == isVaccation) {
        return;
    }
    m_isVaccation = isVaccation;
    emit isVaccationChanged();
    calcWorkTime();
}

void ControllerDay::setStartTime(const QTime &startTime)
{
    if (!setTime(m_startTime, startTime)) {
        return;
    }
    emit startTimeChanged();
    calcWorkTime();
}

void ControllerDay::setEndTime(const QTime &endTime)
{
    if (!setTime(m_endTime, endTime)) {
        return;
    }
    emit endTimeChanged();
    calcWorkTime();
}

void ControllerDay::calcWorkTime()
{
    if (m_isHoliday || m_isVaccation) {
        setWorkTime(m_defaultWorkTime);
    }
    else {
        auto workTime = workTimeFromTimes(m_startTime, m_endTime, m_pauseTime);
        setWorkTime(workTime);
    }
}

void ControllerDay::setWorkTime(const QTime &workTime)
{
    if (m_workTime == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

} // namespace whm
