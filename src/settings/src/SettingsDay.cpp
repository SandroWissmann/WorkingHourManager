#include "../include/whm/settings/SettingsDay.hpp"

namespace whm{

SettingsDay::SettingsDay(
    Weekday weekday,
    Time defaultWorkTime,
    Time pauseTime)
    :m_weekday{weekday}, m_defaultWorkTime{defaultWorkTime}, m_pauseTime{pauseTime}
{

}

Weekday SettingsDay::weekday() const
{
    return m_weekday;
}

QString SettingsDay::weekdayAsString() const
{
    QString weekday = QVariant::fromValue(m_weekday).toString();
    weekday[0] = weekday[0].toUpper();
    return weekday;
}

Time SettingsDay::defaultWorkTime() const
{
    return m_defaultWorkTime;
}

bool SettingsDay::setDefaultWorkTime(const QString &defaultWorkTime)
{
   if( m_defaultWorkTime.asString() == defaultWorkTime) {
       return false;
   }
   m_defaultWorkTime.set(defaultWorkTime);
   return true;
}

Time SettingsDay::pauseTime() const
{
    return m_pauseTime;
}

bool SettingsDay::setPauseTime(const QString &pauseTime)
{
    if( m_pauseTime.asString() == pauseTime) {
        return false;
    }
    m_pauseTime.set(pauseTime);
    return true;
}

}
