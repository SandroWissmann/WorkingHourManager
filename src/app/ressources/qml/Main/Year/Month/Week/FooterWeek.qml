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
            id: workedHoursAndExpected_text
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Worked hours: ") + root.controller.workedTime + "/"
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
