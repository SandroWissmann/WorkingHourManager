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

    property QtObject controller: backend

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Save")
                onTriggered: root.controller.saveToFile()
            }
        }
    }

    header: TabBar {
        id: tabBarYear
    }

    Year {
        anchors.fill: parent
        controller: root.controller.controllerYears[tabBarYear.currentIndex]
    }

    Component {
        id: componentTabButton
        TabButton {}
    }

    Component.onCompleted: {
        for (var i = 0; i < root.controller.controllerYears.length; i++) {
            var year = root.controller.controllerYears[i].year
            var tabYear = componentTabButton.createObject(tabBarYear, {
                                                              "text": year
                                                          })
            tabBarYear.addItem(tabYear)
        }
    }
}
