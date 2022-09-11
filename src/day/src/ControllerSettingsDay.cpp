#include "../include/whm/day/ControllerSettingsDay.hpp"

namespace whm {

ControllerSettingsDay::ControllerSettingsDay(SettingsDay* settingsDay)
    :m_settingsDay{settingsDay}
{

}

SettingsDay ControllerSettingsDay::settingsDay() const
{
    return *m_settingsDay;
}

Weekday ControllerSettingsDay::weekday() const
{
    return m_settingsDay->weekday();
}

QString ControllerSettingsDay::weekdayAsString() const
{
    return m_settingsDay->weekdayAsString();
}

QString ControllerSettingsDay::defaultWorkTimeAsString() const
{
    return m_settingsDay->defaultWorkTime().asString();
}

void ControllerSettingsDay::setDefaultWorkTime(const QString &defaultWorkTime)
{
    if(!m_settingsDay->setDefaultWorkTime(defaultWorkTime)) {
        return;
    }
    emit defaultWorkTimeChanged(m_settingsDay->defaultWorkTime());
}

QString ControllerSettingsDay::pauseTimeAsString() const
{
    return m_settingsDay->pauseTime().asString();
}

void ControllerSettingsDay::setPauseTime(const QString &pauseTime)
{
    if(!m_settingsDay->setPauseTime(pauseTime)) {
        return;
    }
    emit pauseTimeChanged(m_settingsDay->pauseTime());
}


} // namespace whm
