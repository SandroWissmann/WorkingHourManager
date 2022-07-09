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

        property int elementWidth: root.width / 8

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
            readOnly: vacation_checkBox.checked || holiday_checkBox.checked

            onEditingFinished: {
                root.controller.startTime = text
            }
        }
        HourMinInput {
            id: endTime_textField
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.endTime
            readOnly: vacation_checkBox.checked || holiday_checkBox.checked

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
            text: root.controller.workTime
        }
        CheckBox {
            id: holiday_checkBox
            Layout.preferredWidth: rowLayout.elementWidth
            checked: root.controller.isHoliday

            onCheckedChanged: {
                if (checked) {
                    vacation_checkBox.checked = false
                }
                root.controller.isHoliday = checked
            }
        }
        CheckBox {
            id: vacation_checkBox
            Layout.preferredWidth: rowLayout.elementWidth
            checked: root.controller.isVacation

            onCheckedChanged: {
                if (checked) {
                    holiday_checkBox.checked = false
                }
                root.controller.isVacation = checked
            }
        }
    }
}
