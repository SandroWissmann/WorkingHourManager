import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Day"

Item {
    id: root

    property QtObject controller

    function blockTimeInput() {
        startTime_hourMinInput.enabled = false
        endTime_hourMinInput.enabled = false
    }

    function unblockTimeInput() {
        startTime_hourMinInput.enabled = true
        endTime_hourMinInput.enabled = true
    }

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
            readOnly: vacation_checkBox.checked || holiday_checkBox.checked

            onEditingFinished: {
                root.controller.startTime = text
            }
        }
        HourMinInput {
            id: endTime_hourMinInput
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
                    ignore_checkBox.checked = false

                    root.blockTimeInput()
                } else {
                    root.unblockTimeInput()
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
                    ignore_checkBox.checked = false

                    root.blockTimeInput()
                } else {
                    root.unblockTimeInput()
                }

                root.controller.isVacation = checked
            }
        }
        CheckBox {
            id: ignore_checkBox
            Layout.preferredWidth: rowLayout.elementWidth
            checked: root.controller.isIgnore

            onCheckedChanged: {
                if (checked) {
                    holiday_checkBox.checked = false
                    vacation_checkBox.checked = false

                    root.blockTimeInput()
                } else {
                    root.unblockTimeInput()
                }
                root.controller.isIgnore = checked
            }
        }
    }
}
