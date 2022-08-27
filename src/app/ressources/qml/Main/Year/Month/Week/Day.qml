import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Day"

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
            id: workedTime_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            text: root.controller.workedTime
        }
        SpinBox {
            id: dayType_spinBox
            Layout.preferredWidth: rowLayout.elementWidth
            from: 0
            // TODO: Get max value from the backend
            to: items.length - 1
            value: root.controller.dayType
            wrap: true

            // TODO: Provide this list from the backend
            property var items: ["Work", "Holiday", "Vaccation", "Sick", "Party", "Flextime", "Ignore"]

            textFromValue: function (value) {
                return items[value]
            }

            valueFromText: function (text) {
                for (var i = 0; i < items.length; ++i) {
                    if (items[i].toLowerCase().indexOf(
                                text.toLowerCase()) === 0)
                        return i
                }
                return dayType_spinBox.value
            }

            onValueChanged: {
                root.controller.dayType = value
            }
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
    }
}
