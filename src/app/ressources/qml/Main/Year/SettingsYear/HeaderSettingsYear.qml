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
            text: qsTr("Day")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Default work time")
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Pause time")
            font.bold: true
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
    }
}
