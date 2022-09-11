import QtQuick
import QtQuick.Layouts

import "Week"
import "../Day"

Item {
    id: root

    property QtObject controller

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        Repeater {
            model: root.controller.controllerDays

            Day {
                Layout.preferredHeight: columnLayout.elementHeigth
                Layout.preferredWidth: parent.width

                controller: model.modelData
            }
        }
        FooterWeek {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller
        }
    }
}
