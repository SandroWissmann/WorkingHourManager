import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "../Year"

Page {
    id: root

    property QtObject controller

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
