
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

Item {
    id: root

    property QtObject controller

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            id: workHoursAndExpected_text
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Worked hours: ") + root.controller.workTime + "/"
                  + root.controller.expectedWorkedTime
            font.bold: true
        }
        Text {
            id: overtime
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Overtime: ") + root.controller.overtime
            font.bold: true
        }
        Text {
            id: earliestEndTime
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Earliest end time: ") + root.controller.earliestEndTime
            font.bold: true
        }
    }
}
