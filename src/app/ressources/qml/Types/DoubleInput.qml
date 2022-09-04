import QtQuick
import QtQuick.Controls

TextField {
    id: root
    font.bold: true
    validator: RegularExpressionValidator {
        regularExpression: /^(\d)*(\.)?([0-9]{1})?$/
    }
    color: {
        if (!enabled) {
            return "green"
        }
        if (!acceptableInput) {
            return "red"
        }
        return "green"
    }

    onFocusChanged: {
        if (!acceptableInput) {
            text = "0.0"
        }
    }
}
