#include "ControllerSettingsYear.hpp"

#include "ControllerSettingsDay.hpp"

namespace whm {

namespace {

QVector<QObject *> makeControllerSettingsDays(SettingsYear &settingsYear)
{
    return QVector<QObject *>{
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::monday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::tuesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::wednesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::thursday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::friday)}};
}

} // namespace

ControllerSettingsYear::ControllerSettingsYear(const SettingsYear &settingsYear)
    : m_settingsYear(settingsYear),
      m_controllerSettingsDays{makeControllerSettingsDays(m_settingsYear)}
{
}

SettingsYear ControllerSettingsYear::settingsYear() const
{
    return m_settingsYear;
}

QVector<QObject *> ControllerSettingsYear::controllerSettingsDays() const
{
    return m_controllerSettingsDays;
}

double ControllerSettingsYear::flextimeDays()
{
    return m_settingsYear.flextimeDays();
}

QString ControllerSettingsYear::flextimeDaysAsString()
{
    return QString::number(flextimeDays(), 'f', 1);
}

void ControllerSettingsYear::setFlextimeDays(
    const QString &flextimeDaysAsString)
{
    if (!m_settingsYear.setFlextimeDays(flextimeDaysAsString)) {
        return;
    }
    emit flextimeDaysChanged();
}

double ControllerSettingsYear::vaccationDays()
{
    return m_settingsYear.vaccationDays();
}

QString ControllerSettingsYear::vaccationDaysAsString()
{
    return QString::number(vaccationDays(), 'f', 1);
}

void ControllerSettingsYear::setVaccationDays(
    const QString &vaccationDaysAsString)
{
    if (!m_settingsYear.setVaccationDays(vaccationDaysAsString)) {
        return;
    }
    emit vaccationDaysChanged();
}

} // namespace whm
