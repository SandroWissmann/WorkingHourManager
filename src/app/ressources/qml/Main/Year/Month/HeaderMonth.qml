import QtQuick
import QtQuick.Layouts

Item {
    id: root
    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Date")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Day")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Start time")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("End time")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Pause time")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Worked time")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Overtime")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Day type")
            font.bold: true
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
    }
}
