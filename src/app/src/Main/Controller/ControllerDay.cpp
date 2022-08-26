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
    return endTime - startTime - pauseTime;
}
} // namespace

ControllerDay::ControllerDay(
    std::shared_ptr<Day> day,
    const Time &defaultWorkTime,
    const Time &pauseTime)
    : m_day{day}, m_defaultWorkTime{defaultWorkTime}, m_pauseTime{pauseTime},
      m_workTime{
          calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime)}
{
    Q_ASSERT(pauseTime.isValid());
    Q_ASSERT(defaultWorkTime.isValid());
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

Time ControllerDay::defaultWorkTime() const
{
    return m_defaultWorkTime;
}

Time ControllerDay::startTime() const
{
    return m_day->startTime();
}

QString ControllerDay::startTimeAsString() const
{
    return m_day->startTime().asString();
}

void ControllerDay::setStartTime(const QString &startTime)
{
    if (m_day->setStartTime(startTime)) {
        emit startTimeChanged();
        auto workTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workTime);
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
        auto workTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workTime);
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

    auto notWorkDayToWorkDayTransition =
        oldDayType != DayType::work && newDayType == DayType::work;

    if (isWorkDayToNotWorkDayTransition) {
        if (m_day->setStartTime(Time{})) {
            emit startTimeChanged();
        }

        if (m_day->setEndTime(Time{})) {
            emit endTimeChanged();
        }
        setWorkTime(m_defaultWorkTime);
    }
    else if (notWorkDayToWorkDayTransition) {
        auto workTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workTime);
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

void ControllerDay::setWorkTime(const Time &workTime)
{
    if (m_workTime == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

} // namespace whm
