import QtQuick
import QtQuick.Controls

TextField {
    id: root
    font.bold: true
    inputMask: "99:99"
    color: {
        if (!enabled) {
            return "green"
        }
        if (!acceptableInput) {
            return "red"
        }
        if (root.text === "00:00") {
            return "red"
        }
        return "green"
    }
}
