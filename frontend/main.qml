import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Window {
    id: root
    width: 1024
    height: 800
    visible: true
    title: qsTr("Working Hour Manager")

    property QtObject controller: backend

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        Header {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width
        }

        ScrollView {
            id: scrollView
            Layout.fillHeight: true
            Layout.preferredWidth: parent.width

            ColumnLayout {
                //width: Math.max(scrollView.viewport.width, implicitWidth)
                height: children.height

                Repeater {
                    model: root.controller.controllerWeeks

                    delegate: Week {
                        width: root.width
                        height: columnLayout.elementHeigth * 6

                        controller: model.modelData
                    }
                }
            }
        }
        Item {
            Layout.fillHeight: true
        }
    }
}
