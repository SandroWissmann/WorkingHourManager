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
