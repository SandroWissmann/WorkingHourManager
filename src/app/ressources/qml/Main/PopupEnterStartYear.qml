
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
import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: root
    property int currentYear
    property alias selectedYear: spinBoxYear.value

    anchors.centerIn: Overlay.overlay
    standardButtons: Dialog.Ok

    width: 200
    height: 200
    modal: true
    focus: true
    closePolicy: Popup.NoAutoClose

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent

        Text {
            text: qsTr("Enter start year:")
        }

        SpinBox {
            id: spinBoxYear
            from: 1900
            to: root.currentYear
            value: root.currentYear

            editable: true
        }

        Item {
            Layout.fillHeight: true
        }
    }
}
