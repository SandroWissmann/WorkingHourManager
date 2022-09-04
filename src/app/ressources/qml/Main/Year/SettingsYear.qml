import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "SettingsYear"
import "../../Types"

Item {
    id: root

    property QtObject controller

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        HeaderSettingsYear {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
        }

        Repeater {
            model: root.controller.controllerSettingsDays

            SettingsDay {
                Layout.preferredHeight: columnLayout.elementHeigth
                Layout.preferredWidth: parent.width

                controller: model.modelData
            }
        }

        Rectangle {
            Layout.preferredHeight: 2
            Layout.preferredWidth: parent.width
            color: "black"
        }

        FlextimeDays {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
            flextimeDays: controller.flextimeDays
        }

        VaccationDays {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
            vaccationDays: controller.vaccationDays
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
