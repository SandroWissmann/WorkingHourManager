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
#include "../include/whm/settings/ControllerSettings.hpp"

namespace whm {
ControllerSettings::ControllerSettings(
    const Settings &settings,
    QObject *parent)
    : QObject{parent}, m_settings{settings}
{
}

bool ControllerSettings::showMinutesAsFraction() const
{
    return m_settings.showMinutesAsFraction();
}

void ControllerSettings::setShowMinutesAsFraction(bool showMinutesAsFraction)
{
    if (!m_settings.setShowMinutesAsFraction(showMinutesAsFraction)) {
        return;
    }
    emit showMinutesAsFractionChanged(showMinutesAsFraction);
}

Settings ControllerSettings::settings() const
{
    return m_settings;
}

} // namespace whm
