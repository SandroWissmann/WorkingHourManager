import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property QtObject controller

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / 8

        Text {
            id: workedHoursAndExpected_text
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("worked hours: ") + root.controller.workedTime + "/"
                  + root.controller.expectedWorkedTime
            font.bold: true
        }
        Text {
            id: overTime
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("over time: ") + root.controller.overTime
            font.bold: true
        }
        Text {
            id: earliestEndTime
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("earliest end time: ") + root.controller.earliestEndTime
            font.bold: true
        }
    }
}
