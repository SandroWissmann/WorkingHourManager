
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
