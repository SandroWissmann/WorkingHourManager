import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../../../Types"

Item {
    id: root

    property QtObject controller

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length

        Text {
            id: weekday_text
            Layout.preferredWidth: rowLayout.elementWidth
            font.bold: true
            color: "red"
            text: root.controller.weekday
        }
        HourMinInput {
            id: defaultWorkTime_hourMinInput
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.defaultWorkTime

            onEditingFinished: {
                root.controller.defaultWorkTime = text
            }
        }
        HourMinInput {
            id: pauseTime_hourMinInput
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.controller.pauseTime

            onEditingFinished: {
                root.controller.pauseTime = text
            }
        }
        Item {
            Layout.fillWidth: true
        }
    }
}
