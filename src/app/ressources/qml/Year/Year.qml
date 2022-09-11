import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Year"

Page {
    id: root

    property QtObject controller

    header: TabBar {
        id: tabBarMonth

        onCurrentIndexChanged: {
            if (currentIndex < tabBarMonth.contentData.length - 1) {
                loaderPage.setSource("../Month/Month.qml", {
                                         "controller": root.controller.controllerMonths[tabBarMonth.currentIndex]
                                     })
            } else {
                loaderPage.setSource("Year/SettingsYear.qml", {
                                         "controller": root.controller.controllerSettinsYear
                                     })
            }
        }
    }

    Loader {
        id: loaderPage
        anchors.fill: parent
    }

    Component {
        id: componentTabButton
        TabButton {}
    }

    onControllerChanged: {
        var indexTabSettings = tabBarMonth.contentData.length
        tabBarMonth.removeItem(indexTabSettings)
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
        var tabSettings = componentTabButton.createObject(tabBarMonth, {
                                                              "text": qsTr("Settings")
                                                          })
        tabBarMonth.addItem(tabSettings)

        // This hack is necessary because of bug in tab bar.
        // After we create the tab  we expect to have here:
        // tabBarMonth.contentData.length == 13 (months + settings)
        // but for some reason we have
        // tabBarMonth.contentData.length == 1
        // this gets fixed the first time we click on a new tab.
        // so we hit here very dirty 1 and go back to 0.
        tabBarMonth.setCurrentIndex(1)
        tabBarMonth.setCurrentIndex(0)
    }

    footer: FooterYear {
        controller: root.controller
        height: 30
    }
}
