import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property QtObject controller

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        Day {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller.controllerMonday
        }
        Day {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller.controllerTuesday
        }
        Day {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller.controllerWednesday
        }
        Day {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller.controllerThursday
        }
        Day {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller.controllerFriday
        }
        Footer {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller
        }
    }
}
