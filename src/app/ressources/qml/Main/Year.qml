import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Page {
    id: root

    property QtObject controller

    header: TabBar {
        id: tabBarMonth
    }

    Loader {
        id: loaderMonth
        anchors.fill: parent
    }

    Component {
        id: componentMonth

        Month {
            controller: root.controller.controllerMonths[tabBarMonth.currentIndex]
        }
    }

    Component {
        id: componentTabButton
        TabButton {}
    }

    //    Component.onCompleted: {
    //        for (var i = 0; i < root.controller.controllerMonths.length; i++) {
    //            var month = root.controller.controllerMonths[i].month
    //            var tabMonth = componentTabButton.createObject(tabBarMonth, {
    //                                                               "text": month
    //                                                           })
    //            tabBarMonth.addItem(tabMonth)
    //        }
    //    }
    onControllerChanged: {

        loaderMonth.sourceComponent = undefined

        console.warn("controller changed")

        for (var i = tabBarMonth.contentData.length - 1; i >= 0; i--) {
            var object = tabBarMonth.contentData[i]

            console.warn("remove item: " + tabBarMonth)
            tabBarMonth.removeItem(object)
        }
        console.warn("tabBarMonth.contentData.length: " + tabBarMonth.contentData.length)

        for (var j = 0; j < root.controller.controllerMonths.length; j++) {
            var month = root.controller.controllerMonths[j].month
            var tabMonth = componentTabButton.createObject(tabBarMonth, {
                                                               "text": month
                                                           })
            tabBarMonth.addItem(tabMonth)
        }

        loaderMonth.sourceComponent = componentMonth
    }
}
