import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Main"

ApplicationWindow {
    id: root
    width: 1024
    height: 800
    visible: true
    title: qsTr("Working Hour Manager")

    property QtObject backend: backendMain

    Loader {
        id: loader
        anchors.fill: parent
    }

    Component.onCompleted: {
        if (root.backend.readControllerYearsFromFile()) {
            loader.sourceComponent = componentYears
        } else {
            popup.open()
        }
    }

    Component.onDestruction: {
        root.backend.saveToFile()
    }

    Component {
        id: componentYears

        Years {
            controller: root.backend
        }
    }

    PopupEnterStartYear {
        id: popup

        currentYear: root.backend.currentYear

        onAccepted: {
            root.backend.generateControllerYears(popup.selectedYear)
            loader.sourceComponent = componentYears
        }
    }
}
