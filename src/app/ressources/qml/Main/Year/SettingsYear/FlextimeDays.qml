import QtQuick
import QtQuick.Layouts

import "../../../Types"

Item {
    id: root

    property string flextimeDays

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            color: "red"
            text: qsTr("Flextime days: ")
        }

        DoubleInput {
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.flextimeDays

            onEditingFinished: {
                root.flextimeDays = text
            }
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
