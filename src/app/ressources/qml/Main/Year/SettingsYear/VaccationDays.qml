import QtQuick
import QtQuick.Layouts

import "../../../Types"

Item {
    id: root

    property string vaccationDays

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            color: "red"
            text: qsTr("Vaccation days: ")
        }

        DoubleInput {
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.vaccationDays

            onEditingFinished: {
                root.vaccationDays = text
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
