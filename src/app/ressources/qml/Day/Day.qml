
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

import "../Types"

Item {
    id: root

    property QtObject controller

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            id: date_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            color: "red"
            text: root.controller.date
        }
        Text {
            id: weekday_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            color: "red"
            text: root.controller.weekday
        }
        HourMinInput {
            id: startTime_hourMinInput
            enabled: root.controller.timeInputIsEnabled
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.startTime

            // TODO: Understand why we need this. The asignment
            // text: root.controller.startTime should already upate this always?
            onEditingFinished: {
                root.controller.startTime = text
            }
        }
        HourMinInput {
            id: endTime_hourMinInput
            enabled: root.controller.timeInputIsEnabled
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.endTime

            // TODO: Understand why we need this. The asignment
            // text: root.controller.startTime should already upate this always?
            onEditingFinished: {
                root.controller.endTime = text
            }
        }
        Text {
            id: pauseTime_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            text: root.controller.pauseTime
        }
        Text {
            id: workTime_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            text: root.controller.workTime
        }
        Text {
            id: overtime_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            text: root.controller.overtime
        }
        ComboBox {
            id: dayType_spinBox
            Layout.preferredWidth: rowLayout.elementWidth
            currentIndex: root.controller.dayType

            model: root.controller.dayTypesAsVariant

            onCurrentIndexChanged: {
                root.controller.dayType = currentIndex
            }
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
