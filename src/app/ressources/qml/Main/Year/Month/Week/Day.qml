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
            // TODO: Use proper enum value here
            checked: root.controller.dayType === 1

            onCheckedChanged: {
                if (checked) {
                    vacation_checkBox.checked = false
                    ignore_checkBox.checked = false

                    root.blockTimeInput()

                    // TODO: Use proper enum value here
                    root.controller.dayType = 1
                } else {
                    root.unblockTimeInput()

                    if (!holiday_checkBox.checked && !vacation_checkBox.checked
                            && !ignore_checkBox.checked) {
                        // TODO: Use proper enum value here
                        root.controller.dayType = 0
                    }
                }
            }
        }
        CheckBox {
            id: vacation_checkBox
            Layout.preferredWidth: rowLayout.elementWidth
            // TODO: Use proper enum value here
            checked: root.controller.dayType === 2

            onCheckedChanged: {
                if (checked) {
                    holiday_checkBox.checked = false
                    ignore_checkBox.checked = false

                    root.blockTimeInput()

                    // TODO: Use proper enum value here
                    root.controller.dayType = 2
                } else {
                    root.unblockTimeInput()

                    if (!holiday_checkBox.checked && !vacation_checkBox.checked
                            && !ignore_checkBox.checked) {
                        // TODO: Use proper enum value here
                        root.controller.dayType = 0
                    }
                }
            }
        }
        CheckBox {
            id: ignore_checkBox
            Layout.preferredWidth: rowLayout.elementWidth
            // TODO: Use proper enum value here
            checked: root.controller.dayType === 3

            onCheckedChanged: {
                if (checked) {
                    holiday_checkBox.checked = false
                    vacation_checkBox.checked = false

                    root.blockTimeInput()

                    // TODO: Use proper enum value here
                    root.controller.dayType = 3
                } else {
                    root.unblockTimeInput()

                    if (!holiday_checkBox.checked && !vacation_checkBox.checked
                            && !ignore_checkBox.checked) {
                        // TODO: Use proper enum value here
                        root.controller.dayType = 0
                    }
                }
            }
        }
    }
}
