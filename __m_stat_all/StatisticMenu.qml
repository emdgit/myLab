import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

// Form which opens when statistic button pressed
Item {

    PlusButton {
        id: plus

        basePic: "qrc:/img/images/plus.png"
        hoverPic: "qrc:/img/images/plus_shadow.png"
        pressedPic: "qrc:/img/images/plus_shadow_pushed.png"

        anchors.bottom: parent.bottom
        anchors.left: parent.left
    }

//    SwipeView {
//        orientation: Qt.Horizontal
//        Item {
//            id: page1
//            Rectangle {
//                anchors.fill: parent
//                color: "red"
//            }
//        }
//        Item {
//            id: page2
//            Rectangle {
//                anchors.fill: parent
//                color: "green"
//            }
//        }
//        Item {
//            id: page3
//            Rectangle {
//                anchors.fill: parent
//                color: "magenta"
//            }
//        }
//    }

    TreeView {
        id: view

        model: ModelManager.spendModel

        TableViewColumn {
               role: "r_pgroup_name"
               title: "Name"
        }
    }
}
