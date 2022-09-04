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
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Overtime in %1: %2").arg(
                      root.controller.nameOfMonth).arg(root.controller.overtime)
            font.bold: true
        }
    }
}
