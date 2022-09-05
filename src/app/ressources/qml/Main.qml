import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

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
        console.warn("readControllerYearsFromFile")
        if (!root.backend.readControllerYearsFromFile()) {
            console.warn("generateControllerYears")
            root.backend.generateControllerYears()
        }
        loader.sourceComponent = componentYears
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
}
