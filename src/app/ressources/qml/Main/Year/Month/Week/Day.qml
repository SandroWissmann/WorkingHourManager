import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../../../../Types"

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
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.startTime
            // TODO: Use proper enum value here
            // 0 -> work
            enabled: root.controller.dayType === 0

            onEditingFinished: {
                root.controller.startTime = text
            }
        }
        HourMinInput {
            id: endTime_hourMinInput
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.endTime
            enabled: startTime_hourMinInput.enabled

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

            // TODO: provide this list from the backend
            model: ["Work", "Holiday", "Vacation", "Sick", "Party", "Flextime", "Ignore"]

            onCurrentIndexChanged: {
                root.controller.dayType = currentIndex
            }
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
    }
}
