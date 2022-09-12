
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
