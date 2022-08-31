import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "SettingsYear"

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

        Item {
            Layout.fillHeight: true
        }
    }
}
