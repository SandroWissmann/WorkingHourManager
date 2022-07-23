#include "ControllerDay.hpp"

namespace whm {

ControllerDay::ControllerDay(
    const Day &day,
    const Time &defaultWorkTime,
    const Time &pauseTime,
    QObject *parent)
    : QObject{parent}, m_day{day}, m_defaultWorkTime{defaultWorkTime},
      m_pauseTime{pauseTime}
{
    calcWorkTime();
}

Day ControllerDay::day() const
{
    return m_day;
}

QString ControllerDay::dateAsString() const
{
    return m_day.date().asString();
}

QString ControllerDay::weekday() const
{
    return m_day.date().weekday();
}

Time ControllerDay::startTime() const
{
    return m_day.startTime();
}

QString ControllerDay::startTimeAsString() const
{
    return m_day.startTime().asString();
}

void ControllerDay::setStartTime(const QString &startTime)
{
    if (m_day.setStartTime(startTime)) {
        emit startTimeChanged();
        calcWorkTime();
    }
}

QString ControllerDay::endTimeAsString() const
{
    return m_day.endTime().asString();
}

void ControllerDay::setEndTime(const QString &endTime)
{
    if (m_day.setEndTime(endTime)) {
        emit endTimeChanged();
        calcWorkTime();
    }
}

Time ControllerDay::pauseTime() const
{
    return m_pauseTime;
}

QString ControllerDay::pauseTimeAsString() const
{
    return m_pauseTime.asString();
}

Time ControllerDay::workedTime() const
{
    return m_workTime;
}

QString ControllerDay::workTimeAsString() const
{
    return m_workTime.asString();
}

bool ControllerDay::isHoliday() const
{
    return m_day.isHoliday();
}

void ControllerDay::setIsHoliday(bool isHoliday)
{
    if (!m_day.setIsHoliday(isHoliday)) {
        return;
    }
    emit isHolidayChanged();

    if (m_day.setStartTime(Time{})) {
        emit startTimeChanged();
    }

    if (m_day.setEndTime(Time{})) {
        emit endTimeChanged();
    }

    calcWorkTime();
}

bool ControllerDay::isVacation() const
{
    return m_day.isVacation();
}

bool ControllerDay::hasValidStartTime() const
{
    return m_day.hasValidStartTime();
}

bool ControllerDay::hasValidEndTime() const
{
    return m_day.hasValidEndTime();
}

void ControllerDay::setisVacation(bool isVacation)
{
    if (!m_day.setisVacation(isVacation)) {
        return;
    }
    emit isVacationChanged();

    if (m_day.setStartTime(Time{})) {
        emit startTimeChanged();
    }

    if (m_day.setEndTime(Time{})) {
        emit endTimeChanged();
    }

    calcWorkTime();
}

void ControllerDay::calcWorkTime()
{
    Q_ASSERT(m_pauseTime.isValid());

    auto isHoliday = m_day.isHoliday();
    auto isVacation = m_day.isVacation();
    auto startTime = m_day.startTime();
    auto endTime = m_day.endTime();

    if (isHoliday) {
        setWorkTime(m_defaultWorkTime);
    }
    else if (isVacation) {
        setWorkTime(m_defaultWorkTime);
    }
    else if (!startTime.isValid()) {
        setWorkTime(Time{});
    }
    else if (!endTime.isValid()) {
        setWorkTime(Time{});
    }
    else {
        auto workTime = m_day.endTime() - m_day.startTime() - m_pauseTime;
        setWorkTime(workTime);
    }
}

void ControllerDay::setWorkTime(const Time &workTime)
{
    if (m_workTime == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

} // namespace whm
