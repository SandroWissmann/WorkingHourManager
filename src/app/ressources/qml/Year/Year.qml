
/* Working Hour Manager
 * Copyright (C) 2022 Sandro Wißmann
 *
 * Working Hour Manager is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Working Hour Manager is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Quiz If not, see <http://www.gnu.org/licenses/>.
 *
 * Web-Site: https://github.com/SandroWissmann/WorkingHourManager
 */
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
                                         "controller": root.controller.controllerSettingsYear
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
        var indexTabSettingsYear = tabBarMonth.contentData.length
        tabBarMonth.takeItem(indexTabSettingsYear)
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
        var tabSettingsYear = componentTabButton.createObject(tabBarMonth, {
                                                                  "text": qsTr("Settings")
                                                              })
        tabBarMonth.addItem(tabSettingsYear)

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
