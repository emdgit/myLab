import QtQuick 2.0
import QtQuick.Controls 1.4

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

    TreeView {

        model: ModelManager.spendModel

    }

}
