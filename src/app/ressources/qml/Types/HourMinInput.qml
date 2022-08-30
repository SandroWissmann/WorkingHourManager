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
