import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Year"

Page {
    id: root

    property QtObject controller

    header: TabBar {
        id: tabBarMonth
    }

    Loader {
        id: loaderMonth
        anchors.fill: parent
    }

    Component {
        id: componentMonth

        Month {
            controller: root.controller.controllerMonths[tabBarMonth.currentIndex]
        }
    }

    Component {
        id: componentTabButton
        TabButton {}
    }

    onControllerChanged: {
        loaderMonth.sourceComponent = undefined

        for (var i = tabBarMonth.contentData.length - 1; i >= 0; i--) {
            var object = tabBarMonth.contentData[i]
            tabBarMonth.removeItem(object)
        }

        for (var j = 0; j < root.controller.controllerMonths.length; j++) {
            var month = root.controller.controllerMonths[j].nameOfMonth
            var tabMonth = componentTabButton.createObject(tabBarMonth, {
                                                               "text": month
                                                           })
            tabBarMonth.addItem(tabMonth)
        }

        loaderMonth.sourceComponent = componentMonth
    }
}
