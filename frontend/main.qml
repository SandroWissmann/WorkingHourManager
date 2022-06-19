import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: root
    width: 1024
    height: 800
    visible: true
    title: qsTr("Working Hour Manager")

    property QtObject controller: controllerWeek

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        Header {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
        }

        Week {
            Layout.preferredHeight: columnLayout.elementHeigth * 6
            Layout.preferredWidth: parent.width

            controller: root.controller
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
