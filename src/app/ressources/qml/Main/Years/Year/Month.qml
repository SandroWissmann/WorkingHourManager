import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

import "Month"
import "../../../Week"

Item {
    id: root

    property QtObject controller

    ColumnLayout {
        id: columnLayout
        anchors.fill: parent
        spacing: 0

        property int elementHeigth: 30

        HeaderMonth {
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
        FooterMonth {
            Layout.preferredHeight: columnLayout.elementHeigth
            Layout.preferredWidth: parent.width

            controller: root.controller
        }
    }
}
