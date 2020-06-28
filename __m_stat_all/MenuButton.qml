import QtQuick 2.0
import QtQuick.Controls 2.2

import "Common.js" as Script

Button {

    property int number: -1

    id:     menuButton
    width:  80
    height: 80

    hoverEnabled: true

    background: Rectangle {
        color: menuButton.hovered ? Script.hoveredColor()
                                  : "transparent"
    }

    Rectangle {
        color:          "transparent"
        anchors.fill:   parent
    }
}

