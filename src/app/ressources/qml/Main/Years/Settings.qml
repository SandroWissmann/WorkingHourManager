import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    property QtObject controller

    anchors.fill: parent

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        Switch {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            text: qsTr("Minutes as fraction")
            checked: root.controller.showMinutesAsFraction

            onCheckedChanged: {
                root.controller.showMinutesAsFraction = checked
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
