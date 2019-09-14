import QtQuick 2.0
import QtQuick.Controls 2.2

Button {

    id:     menuButton
    width:  80
    height: 80

    background:
        Rectangle {
            color: menuButton.hovered ? "#3A2E5C"
                                      : "transparent"
    }

    Rectangle {

        color:          "transparent"
        anchors.fill:   parent
    }
}

