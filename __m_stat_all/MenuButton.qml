import QtQuick 2.0
import QtQuick.Controls 2.2

Button{

    property url imgSource: ""

    id: menuButton
    width: 80
    height: 80

    Image {
        id: img
        source: parent.imgSource
        anchors.fill: parent
    }
}

