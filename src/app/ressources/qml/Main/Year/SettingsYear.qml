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

        SettingsDay {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            weekday: qsTr("Monday")
            defaultWorkTime: root.controller.defaultWorkTimeMonday
            pauseTime: root.controller.pauseTimeMonday
        }

        SettingsDay {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            weekday: qsTr("Tuesday")
            defaultWorkTime: root.controller.defaultWorkTimeTuesday
            pauseTime: root.controller.pauseTimeTuesday
        }

        SettingsDay {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            weekday: qsTr("Wednesday")
            defaultWorkTime: root.controller.defaultWorkTimeWednesday
            pauseTime: root.controller.pauseTimeWednesday
        }

        SettingsDay {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            weekday: qsTr("Thursday")
            defaultWorkTime: root.controller.defaultWorkTimeThursday
            pauseTime: root.controller.pauseTimeThursday
        }

        SettingsDay {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            weekday: qsTr("Friday")
            defaultWorkTime: root.controller.defaultWorkTimeFriday
            pauseTime: root.controller.pauseTimeFriday
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
