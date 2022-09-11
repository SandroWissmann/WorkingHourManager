#include "../include/whm/year/ControllerSettingsYear.hpp"

#include <whm/day/ControllerSettingsDay.hpp>

namespace whm {

namespace {

QVector<QObject *> makeControllerSettingsDays(SettingsYear &settingsYear)
{
    return QVector<QObject *>{
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Monday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Tuesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Wednesday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Thursday)},
        new ControllerSettingsDay{settingsYear.settingsDay(Weekday::Friday)}};
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

double ControllerSettingsYear::vacationDays()
{
    return m_settingsYear.vacationDays();
}

QString ControllerSettingsYear::vacationDaysAsString()
{
    return QString::number(vacationDays(), 'f', 1);
}

void ControllerSettingsYear::setVacationDays(
    const QString &vacationDaysAsString)
{
    if (!m_settingsYear.setVacationDays(vacationDaysAsString)) {
        return;
    }
    emit vacationDaysChanged();
}

} // namespace whm
