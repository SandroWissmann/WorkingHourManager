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
#ifndef WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_HPP
#define WORKING_HOUR_MANAGER_CONTROLLER_SETTINGS_HPP

#include "Settings.hpp"

#include <QObject>

namespace whm {

class ControllerSettings : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool showMinutesAsFraction READ showMinutesAsFraction WRITE
                   setShowMinutesAsFraction NOTIFY showMinutesAsFractionChanged)
public:
    ControllerSettings(const Settings &settings, QObject *parent = nullptr);

    ControllerSettings(const ControllerSettings &) = delete;
    ControllerSettings(ControllerSettings &&) = delete;
    ControllerSettings &operator=(const ControllerSettings &) = delete;
    ControllerSettings &operator=(ControllerSettings &&) = delete;

    bool showMinutesAsFraction() const;
    void setShowMinutesAsFraction(bool showMinutesAsFraction);

    Settings settings() const;

signals:
    void showMinutesAsFractionChanged(bool showMinutesAsFraction);

private:
    Settings m_settings;
};

} // namespace whm

#endif
