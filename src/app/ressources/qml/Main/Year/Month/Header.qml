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
            text: qsTr("Start hh:mm")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("End hh:mm")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Pause hh:mm")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Worked hh:mm")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Holiday")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Vacation")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Ignore")
            font.bold: true
        }
    }
}
