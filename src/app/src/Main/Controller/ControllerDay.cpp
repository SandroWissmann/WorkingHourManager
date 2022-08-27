#include "ControllerDay.hpp"

#include <whm/types/Day.hpp>

namespace whm {

namespace {
Time calcWorkTime(Time startTime, Time endTime, Time pauseTime)
{
    auto timesAreValid = startTime.isValid() && endTime.isValid();

    if (!timesAreValid) {
        return Time{};
    }
    auto workTime = endTime - startTime - pauseTime;

    // If result would be negative time gets invalid and has empty string so
    // we correct it to 0:00
    if (workTime.asString().isEmpty()) {
        return Time{};
    }
    return workTime;
}
} // namespace

ControllerDay::ControllerDay(
    std::shared_ptr<Day> day,
    const Time &defaultWorkedTime,
    const Time &pauseTime)
    : m_day{day}, m_defaultWorkedTime{defaultWorkedTime},
      m_pauseTime{pauseTime},
      m_workedTime{
          calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime)}
{
    Q_ASSERT(pauseTime.isValid());
    Q_ASSERT(defaultWorkedTime.isValid());
}

ControllerDay::~ControllerDay() = default;

std::shared_ptr<Day> ControllerDay::day() const
{
    return m_day;
}

QString ControllerDay::dateAsString() const
{
    return m_day->date().asString();
}

QString ControllerDay::weekday() const
{
    return m_day->date().weekday();
}

Time ControllerDay::defaultWorkedTime() const
{
    return m_defaultWorkedTime;
}

Time ControllerDay::startTime() const
{
    return m_day->startTime();
}

QString ControllerDay::startTimeAsString() const
{
    return startTime().asString();
}

void ControllerDay::setStartTime(const QString &startTime)
{
    if (m_day->setStartTime(startTime)) {
        emit startTimeChanged();
        auto workedTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workedTime);
    }
}

QString ControllerDay::endTimeAsString() const
{
    return m_day->endTime().asString();
}

void ControllerDay::setEndTime(const QString &endTime)
{
    if (m_day->setEndTime(endTime)) {
        emit endTimeChanged();
        auto workedTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        qDebug() << "time" << workedTime.asString();
        setWorkTime(workedTime);
    }
}

Time ControllerDay::pauseTime() const
{
    return m_pauseTime;
}

QString ControllerDay::pauseTimeAsString() const
{
    return pauseTime().asString();
}

Time ControllerDay::workedTime() const
{
    if (m_day->dayType() == DayType::flextime) {
        return Time{};
    }
    if (m_day->dayType() != DayType::work) {
        return m_defaultWorkedTime;
    }
    return m_workedTime;
}

QString ControllerDay::workedTimeAsString() const
{
    return workedTime().asString();
}

DayType ControllerDay::dayType() const
{
    return m_day->dayType();
}

void ControllerDay::setDayType(DayType dayType)
{
    auto oldDayType = m_day->dayType();

    if (!m_day->setDayType(dayType)) {
        return;
    }
    emit dayTypeChanged();

    auto newDayType = m_day->dayType();

    auto isWorkDayToNotWorkDayTransition =
        oldDayType == DayType::work && newDayType != DayType::work;

    auto isNotWorkDayToWorkDayTransition =
        oldDayType != DayType::work && newDayType == DayType::work;

    auto isNotFlextimeToFlextimeTransition =
        oldDayType != DayType::flextime && newDayType == DayType::flextime;

    auto isFlextimeToNotFlextimeTransition =
        oldDayType == DayType::flextime && newDayType != DayType::flextime;

    if (isWorkDayToNotWorkDayTransition) {
        emit startTimeChanged();
        emit endTimeChanged();
        emit workedTimeChanged();
    }
    else if (isNotWorkDayToWorkDayTransition) {
        emit startTimeChanged();
        emit endTimeChanged();

        auto workedTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workedTime);
        // Even if setWorkTime can emit workedTimeChanged we need it here in
        // case
        emit workedTimeChanged();
    }
    else if (isNotFlextimeToFlextimeTransition) {
        emit workedTimeChanged();
    }
    else if (isFlextimeToNotFlextimeTransition) {
        emit workedTimeChanged();
    }
}

bool ControllerDay::hasValidStartTime() const
{
    return m_day->hasValidStartTime();
}

bool ControllerDay::hasValidEndTime() const
{
    return m_day->hasValidEndTime();
}

void ControllerDay::setWorkTime(const Time &workedTime)
{
    if (this->workedTime() == workedTime) {
        return;
    }
    m_workedTime = workedTime;
    emit workedTimeChanged();
}

} // namespace whm
