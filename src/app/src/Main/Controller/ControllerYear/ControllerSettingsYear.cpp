#include "ControllerSettingsYear.hpp"

namespace whm {
ControllerSettingsYear::ControllerSettingsYear(const SettingsYear &settingsYear)
    : m_settingsYear(settingsYear)
{
}

SettingsYear ControllerSettingsYear::settingsYear() const
{
    return m_settingsYear;
}

QString ControllerSettingsYear::defaultWorkTimeMondayAsString() const
{
    return m_settingsYear.defaultWorkTimeMonday().asString();
}
QString ControllerSettingsYear::defaultWorkTimeTuesdayAsString() const
{
    return m_settingsYear.defaultWorkTimeTuesday().asString();
}
QString ControllerSettingsYear::defaultWorkTimeWednesdayAsString() const
{
    return m_settingsYear.defaultWorkTimeWednesday().asString();
}
QString ControllerSettingsYear::defaultWorkTimeThursdayAsString() const
{
    return m_settingsYear.defaultWorkTimeThursday().asString();
}
QString ControllerSettingsYear::defaultWorkTimeFridayAsString() const
{
    return m_settingsYear.defaultWorkTimeFriday().asString();
}

QString ControllerSettingsYear::pauseTimeMondayAsString() const
{
    return m_settingsYear.pauseTimeMonday().asString();
}
QString ControllerSettingsYear::pauseTimeTuesdayAsString() const
{
    return m_settingsYear.pauseTimeTuesday().asString();
}
QString ControllerSettingsYear::pauseTimeWednesdayAsString() const
{
    return m_settingsYear.pauseTimeWednesday().asString();
}
QString ControllerSettingsYear::pauseTimeThursdayAsString() const
{
    return m_settingsYear.pauseTimeThursday().asString();
}
QString ControllerSettingsYear::pauseTimeFridayAsString() const
{
    return m_settingsYear.pauseTimeFriday().asString();
}

void ControllerSettingsYear::setDefaultWorkTimeMonday(
    const QString &defaultWorkTime)
{
    if (m_settingsYear.setDefaultWorkTimeMonday(defaultWorkTime)) {
        emit defaultWorkTimeMondayAsStringChanged();
        emit defaultWorkTimeMondayChanged(
            m_settingsYear.defaultWorkTimeMonday());
    }
}
void ControllerSettingsYear::setDefaultWorkTimeTuesday(
    const QString &defaultWorkTime)
{
    if (m_settingsYear.setDefaultWorkTimeTuesday(defaultWorkTime)) {
        emit defaultWorkTimeTuesdayAsStringChanged();
        emit defaultWorkTimeTuesdayChanged(
            m_settingsYear.defaultWorkTimeTuesday());
    }
}
void ControllerSettingsYear::setDefaultWorkTimeWednesday(
    const QString &defaultWorkTime)
{
    if (m_settingsYear.setDefaultWorkTimeWednesday(defaultWorkTime)) {
        emit defaultWorkTimeWednesdayAsStringChanged();
        emit defaultWorkTimeWednesdayChanged(
            m_settingsYear.defaultWorkTimeWednesday());
    }
}
void ControllerSettingsYear::setDefaultWorkTimeThursday(
    const QString &defaultWorkTime)
{
    if (m_settingsYear.setDefaultWorkTimeThursday(defaultWorkTime)) {
        emit defaultWorkTimeThursdayAsStringChanged();
        emit defaultWorkTimeThursdayChanged(
            m_settingsYear.defaultWorkTimeThursday());
    }
}
void ControllerSettingsYear::setDefaultWorkTimeFriday(
    const QString &defaultWorkTime)
{
    if (m_settingsYear.setDefaultWorkTimeFriday(defaultWorkTime)) {
        emit defaultWorkTimeFridayAsStringChanged();
        emit defaultWorkTimeFridayChanged(
            m_settingsYear.defaultWorkTimeFriday());
    }
}

void ControllerSettingsYear::setPauseTimeMonday(const QString &pauseTime)
{
    if (m_settingsYear.setPauseTimeMonday(pauseTime)) {
        emit pauseTimeMondayAsStringChanged();
        emit pauseTimeMondayChanged(m_settingsYear.pauseTimeMonday());
    }
}
void ControllerSettingsYear::setPauseTimeTuesday(const QString &pauseTime)
{
    if (m_settingsYear.setPauseTimeTuesday(pauseTime)) {
        emit pauseTimeTuesdayAsStringChanged();
        emit pauseTimeTuesdayChanged(m_settingsYear.pauseTimeTuesday());
    }
}
void ControllerSettingsYear::setPauseTimeWednesday(const QString &pauseTime)
{
    if (m_settingsYear.setPauseTimeWednesday(pauseTime)) {
        emit pauseTimeWednesdayAsStringChanged();
        emit pauseTimeWednesdayChanged(m_settingsYear.pauseTimeWednesday());
    }
}
void ControllerSettingsYear::setPauseTimeThursday(const QString &pauseTime)
{
    if (m_settingsYear.setPauseTimeThursday(pauseTime)) {
        emit pauseTimeThursdayAsStringChanged();
        emit pauseTimeThursdayChanged(m_settingsYear.pauseTimeThursday());
    }
}
void ControllerSettingsYear::setPauseTimeFriday(const QString &pauseTime)
{
    if (m_settingsYear.setPauseTimeFriday(pauseTime)) {
        emit pauseTimeFridayAsStringChanged();
        emit pauseTimeFridayChanged(m_settingsYear.pauseTimeFriday());
    }
}
} // namespace whm
