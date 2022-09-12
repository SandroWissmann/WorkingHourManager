
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
import QtQuick.Controls

TextField {
    id: root
    font.bold: true
    validator: RegularExpressionValidator {
        regularExpression: /([0123456789]|[1][0123456789]|[2][0123])([:]\d{2})?$/
    }
    color: {
        if (!enabled) {
            return "green"
        }
        if (!acceptableInput) {
            return "red"
        }
        if (root.text === "0:00") {
            return "red"
        }
        return "green"
    }

    onFocusChanged: {
        if (!acceptableInput) {
            text = "0:00"
        }
    }
}
