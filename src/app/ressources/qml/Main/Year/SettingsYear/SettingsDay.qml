import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../../../Types"

Item {
    id: root

    // TODO: We should make a ControllerSettingsDay in backend to provide
    // it here as controller
    property string weekday
    property string defaultWorkTime
    property string pauseTime

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
            text: root.weekday
        }
        HourMinInput {
            id: defaultWorkTime_hourMinInput
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.defaultWorkTime

            onEditingFinished: {
                root.defaultWorkTime = text
            }
        }
        HourMinInput {
            id: pauseTime_hourMinInput
            Layout.preferredWidth: rowLayout.elementWidth
            text: root.pauseTime

            onEditingFinished: {
                root.pauseTime = text
            }
        }
        Item {
            Layout.preferredWidth: rowLayout.elementWidth
        }
    }
}
