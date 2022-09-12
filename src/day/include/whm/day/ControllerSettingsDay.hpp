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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_DAY_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_DAY_HPP

#include "SettingsDay.hpp"

#include <whm/types/Time.hpp>

#include <QObject>

#include <vector>

namespace whm {

class ControllerSettingsDay : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString weekday READ weekdayAsString CONSTANT)

    Q_PROPERTY(QString defaultWorkTime READ defaultWorkTimeAsString WRITE
                   setDefaultWorkTime NOTIFY defaultWorkTimeChanged)

    Q_PROPERTY(QString pauseTime READ pauseTimeAsString WRITE setPauseTime
                   NOTIFY pauseTimeChanged)
public:
    ControllerSettingsDay(SettingsDay *settingsDay);

    ControllerSettingsDay(const ControllerSettingsDay &) = delete;
    ControllerSettingsDay(ControllerSettingsDay &&) = delete;
    ControllerSettingsDay &operator=(const ControllerSettingsDay &) = delete;
    ControllerSettingsDay &operator=(ControllerSettingsDay &&) = delete;

    SettingsDay settingsDay() const;

    Weekday weekday() const;
    QString weekdayAsString() const;

    QString defaultWorkTimeAsString() const;
    void setDefaultWorkTime(const QString &defaultWorkTime);

    QString pauseTimeAsString() const;
    void setPauseTime(const QString &pauseTime);

signals:
    // parameter needed here because we catch signal in ControllerDay.
    void defaultWorkTimeChanged(const whm::Time &defaultWorkTime);
    void pauseTimeChanged(const whm::Time &pauseTime);

private:
    SettingsDay *m_settingsDay;
};

} // namespace whm

#endif
