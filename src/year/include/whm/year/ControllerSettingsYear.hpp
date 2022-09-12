/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_YEAR_HPP

#include "SettingsYear.hpp"

#include <whm/types/Time.hpp>

#include <QObject>

#include <vector>

namespace whm {

class ControllerSettingsYear : public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector<QObject *> controllerSettingsDays READ
                   controllerSettingsDays CONSTANT)
    Q_PROPERTY(QString flextimeDays READ flextimeDaysAsString WRITE
                   setFlextimeDays NOTIFY flextimeDaysChanged)
    Q_PROPERTY(QString vacationDays READ vacationDaysAsString WRITE
                   setVacationDays NOTIFY vacationDaysChanged)
public:
    ControllerSettingsYear(const SettingsYear &settingsYear);

    ControllerSettingsYear(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear(ControllerSettingsYear &&) = delete;
    ControllerSettingsYear &operator=(const ControllerSettingsYear &) = delete;
    ControllerSettingsYear &operator=(ControllerSettingsYear &&) = delete;

    SettingsYear settingsYear() const;

    QVector<QObject *> controllerSettingsDays() const;

    double flextimeDays();
    QString flextimeDaysAsString();
    void setFlextimeDays(const QString &flextimeDaysAsString);

    double vacationDays();
    QString vacationDaysAsString();
    void setVacationDays(const QString &vacationDaysAsString);

signals:
    void flextimeDaysChanged();
    void vacationDaysChanged();

private:
    SettingsYear m_settingsYear;
    QVector<QObject *> m_controllerSettingsDays;
};

} // namespace whm

#endif
