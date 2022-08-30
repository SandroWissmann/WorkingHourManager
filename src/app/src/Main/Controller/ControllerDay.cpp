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
    return startTime().asString();
}

void ControllerDay::setStartTime(const QString &startTime)
{
    if (m_day->setStartTime(startTime)) {
        emit startTimeChanged();
        auto workTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workTime);
        emit overtimeChanged();
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
        qDebug() << "time" << workTime.asString();
        setWorkTime(workTime);
        emit overtimeChanged();
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

Time ControllerDay::workTime() const
{
    if (m_day->dayType() != DayType::work) {
        return m_defaultWorkTime;
    }
    return m_workTime;
}

HoursAndMinutes ControllerDay::overtime() const
{
    if (m_day->dayType() == DayType::flextime) {
        return HoursAndMinutes{-m_defaultWorkTime.totalMinutes()};
    }
    if (m_day->dayType() != DayType::work) {
        return HoursAndMinutes{};
    }
    if (!m_day->isValid()) {
        return HoursAndMinutes{};
    }
    return HoursAndMinutes{m_workTime} - HoursAndMinutes{m_defaultWorkTime};
}

QString ControllerDay::overtimeAsString() const
{
    return overtime().toString();
}

QString ControllerDay::workTimeAsString() const
{
    return workTime().asString();
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
        emit workTimeChanged();
        emit defaultWorkTimeChanged();
        emit overtimeChanged();
    }
    else if (isNotWorkDayToWorkDayTransition) {
        emit startTimeChanged();
        emit endTimeChanged();

        auto workTime =
            calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
        setWorkTime(workTime);
        // Even if setWorkTime can emit workTimeChanged we need it here in
        // case
        emit workTimeChanged();
        emit defaultWorkTimeChanged();
        emit overtimeChanged();
    }
    else if (isNotFlextimeToFlextimeTransition) {
        emit workTimeChanged();
        emit defaultWorkTimeChanged();
        emit overtimeChanged();
    }
    else if (isFlextimeToNotFlextimeTransition) {
        emit workTimeChanged();
        emit defaultWorkTimeChanged();
        emit overtimeChanged();
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

void ControllerDay::onDefaultWorkTimeChanged(const whm::Time &defaultWorkTime)
{
    setDefaultWorkTime(defaultWorkTime);
    emit overtimeChanged();
}

void ControllerDay::onPauseTimeChanged(const whm::Time &pauseTime)
{
    setPauseTime(pauseTime);

    auto workTime =
        calcWorkTime(m_day->startTime(), m_day->endTime(), m_pauseTime);
    setWorkTime(workTime);
    emit overtimeChanged();
}

void ControllerDay::setPauseTime(const Time &pauseTime)
{
    if (m_pauseTime == pauseTime) {
        return;
    }
    m_pauseTime = pauseTime;
    emit pauseTimeChanged();
}

void ControllerDay::setWorkTime(const Time &workTime)
{
    if (this->workTime() == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

void ControllerDay::setDefaultWorkTime(const Time &defaultWorkTime)
{
    if (m_defaultWorkTime == defaultWorkTime) {
        return;
    }
    m_defaultWorkTime = defaultWorkTime;
    emit defaultWorkTimeChanged();
}

} // namespace whm
