import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property QtObject controller

    RowLayout {
        id: rowLayout
        anchors.fill: parent
        spacing: 0

        property int elementWidth: root.width / rowLayout.children.length
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Overtime in %1: %2").arg(root.controller.year).arg(
                      root.controller.overtime)
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Flextime days: %1/%2").arg(
                      root.controller.usedFlextimeDays).arg(
                      root.controller.controllerSettinsYear.flextimeDays)
            font.bold: true
        }
        Text {
            Layout.preferredWidth: rowLayout.elementWidth
            text: qsTr("Vacation days: %1/%2").arg(
                      root.controller.usedVacationDays).arg(
                      root.controller.controllerSettinsYear.vacationDays)
            font.bold: true
        }
    }
}
