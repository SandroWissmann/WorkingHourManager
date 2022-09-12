
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
import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "SettingsYear"
import "../../Types"

Item {
    id: root

    property QtObject controller

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        HeaderSettingsYear {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
        }

        Repeater {
            model: root.controller.controllerSettingsDays

            SettingsDay {
                Layout.preferredHeight: columnLayout.elementHeigth
                Layout.preferredWidth: parent.width

                controller: model.modelData
            }
        }

        Rectangle {
            Layout.preferredHeight: 2
            Layout.preferredWidth: parent.width
            color: "black"
        }

        FlextimeDays {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
            controller: root.controller
        }

        VacationDays {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
            controller: root.controller
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
