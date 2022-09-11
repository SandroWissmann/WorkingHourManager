#include "../include/whm/day/ControllerDay.hpp"

#include "../include/whm/day/Day.hpp"

#include "ControllerDay/ControllerDayState.hpp"
#include "ControllerDay/ControllerDayStateFlextime.hpp"
#include "ControllerDay/ControllerDayStateHoliday.hpp"
#include "ControllerDay/ControllerDayStateIgnore.hpp"
#include "ControllerDay/ControllerDayStateParty.hpp"
#include "ControllerDay/ControllerDayStateSick.hpp"
#include "ControllerDay/ControllerDayStateVacation.hpp"
#include "ControllerDay/ControllerDayStateWork.hpp"

namespace whm {

namespace {

ControllerDayState *dayTypeAsState(DayType dayType)
{
    switch (dayType) {
        case DayType::Work:
            return &ControllerDayStateWork::getInstance();
        case DayType::Holiday:
            return &ControllerDayStateHoliday::getInstance();
        case DayType::Vacation:
            return &ControllerDayStateVacation::getInstance();
        case DayType::Sick:
            return &ControllerDayStateSick::getInstance();
        case DayType::Party:
            return &ControllerDayStateParty::getInstance();
        case DayType::Flextime:
            return &ControllerDayStateFlextime::getInstance();
        case DayType::Ignore:
            return &ControllerDayStateIgnore::getInstance();
    }
    return &ControllerDayStateWork::getInstance();
}

QString to24HoursFormat(
    const QString &endTimeAsString,
    const QString &startTimeAsString)
{
    Time startTime{startTimeAsString};
    Time endTime{endTimeAsString};

    if (endTime.isValid() && endTime < startTime && endTime.hour() < 12) {
        auto hour = endTime.hour();
        auto minute = endTime.minute();
        hour += 12;
        endTime = Time{hour, minute};
    }

    return endTime.asString();
}

} // namespace

ControllerDay::ControllerDay(
    std::shared_ptr<Day> day,
    const Time &defaultWorkTime,
    const Time &pauseTime)
    : m_day{day}, m_defaultWorkTime{defaultWorkTime}, m_pauseTime{pauseTime},
      m_enteredStartTime{m_day->startTime().asString()},
      m_enteredEndTime{m_day->endTime().asString()}
{
    auto dayType = day->dayType();
    auto state = dayTypeAsState(dayType);
    setState(state);

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

QString ControllerDay::weekdayAsString() const
{
    auto weekdayEnum = m_day->date().weekday();
    QString weekday = QVariant::fromValue(weekdayEnum).toString();
    return weekday;
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

void ControllerDay::setEnteredStartTime(const QString &enteredStartTime)
{
    if (m_enteredStartTime == enteredStartTime) {
        return;
    }
    m_enteredStartTime = enteredStartTime;
    m_state->calculate(this);
}

QString ControllerDay::enteredStartTime() const
{
    return m_enteredStartTime;
}

Time ControllerDay::endTime() const
{
    return m_day->endTime();
}

QString ControllerDay::endTimeAsString() const
{
    return m_day->endTime().asString();
}

void ControllerDay::setEnteredEndTime(QString enteredEndTime)
{
    enteredEndTime = to24HoursFormat(enteredEndTime, m_enteredStartTime);

    if (m_enteredEndTime == enteredEndTime) {
        return;
    }
    m_enteredEndTime = enteredEndTime;
    m_state->calculate(this);
}

QString ControllerDay::enteredEndTime()
{
    return m_enteredEndTime;
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
    return m_workTime;
}

HoursAndMinutes ControllerDay::overtime() const
{
    return m_overtime;
}

QString ControllerDay::overtimeAsString() const
{
    return overtime().toString();
}

bool ControllerDay::timeInputIsEnabled() const
{
    return m_timeInputIsEnabled;
}

QString ControllerDay::workTimeAsString() const
{
    return workTime().asString();
}

DayType ControllerDay::dayType() const
{
    return m_day->dayType();
}

QVector<QVariant> ControllerDay::dayTypesAsVariant() const
{
    auto stringList = dayTypeAsStringList();
    QVector<QVariant> variantList(stringList.size());
    std::copy(stringList.begin(), stringList.end(), variantList.begin());

    return variantList;
}

void ControllerDay::setDayType(DayType dayType)
{
    if (!m_day->setDayType(dayType)) {
        return;
    }
    emit dayTypeChanged();

    auto state = dayTypeAsState(dayType);
    setState(state);
}

bool ControllerDay::hasValidStartTime() const
{
    return m_day->hasValidStartTime();
}

bool ControllerDay::hasValidEndTime() const
{
    return m_day->hasValidEndTime();
}

double ControllerDay::usedFlextimeDay() const
{
    return m_usedFlextimeDay;
}

double ControllerDay::usedVacationDay() const
{
    return m_usedVacationDay;
}

void ControllerDay::setState(ControllerDayState *state)
{
    m_state = state;
    m_state->calculate(this);
}

void ControllerDay::onDefaultWorkTimeChanged(const whm::Time &defaultWorkTime)
{
    if (m_defaultWorkTime == defaultWorkTime) {
        return;
    }
    m_defaultWorkTime = defaultWorkTime;
    emit defaultWorkTimeChanged();

    m_state->calculate(this);
}

void ControllerDay::onPauseTimeChanged(const whm::Time &pauseTime)
{
    if (m_pauseTime == pauseTime) {
        return;
    }
    m_pauseTime = pauseTime;
    emit pauseTimeChanged();

    m_state->calculate(this);
}

void ControllerDay::setStartTime(const QString &startTime)
{
    if (!m_day->setStartTime(startTime)) {
        return;
    }
    emit startTimeChanged();
}

void ControllerDay::setEndTime(const QString &endTime)
{
    if (!m_day->setEndTime(endTime)) {
        return;
    }
    emit endTimeChanged();
}

void ControllerDay::setWorkTime(const Time &workTime)
{
    if (this->workTime() == workTime) {
        return;
    }
    m_workTime = workTime;
    emit workTimeChanged();
}

void ControllerDay::setOvertime(const HoursAndMinutes &overtime)
{
    if (m_overtime == overtime) {
        return;
    }
    m_overtime = overtime;
    emit overtimeChanged();
}

void ControllerDay::setTimeInputIsEnabled(bool timeInputIsEnabled)
{
    if (m_timeInputIsEnabled == timeInputIsEnabled) {
        return;
    }
    m_timeInputIsEnabled = timeInputIsEnabled;
    emit timeInputIsEnabledChanged();
}

void ControllerDay::setUsedFlextimeDay(double usedFlextimeDay)
{
    Q_ASSERT(usedFlextimeDay >= 0.0);
    Q_ASSERT(usedFlextimeDay <= 1.0);

    if (qFuzzyCompare(m_usedFlextimeDay, usedFlextimeDay)) {
        return;
    }
    m_usedFlextimeDay = usedFlextimeDay;
    emit usedFlextimeDayChanged();
}

void ControllerDay::setUsedVacationDay(double usedVacationDay)
{
    Q_ASSERT(usedVacationDay >= 0.0);
    Q_ASSERT(usedVacationDay <= 1.0);

    if (qFuzzyCompare(m_usedVacationDay, usedVacationDay)) {
        return;
    }
    m_usedVacationDay = usedVacationDay;
    emit usedVacationDayChanged();
}

} // namespace whm
