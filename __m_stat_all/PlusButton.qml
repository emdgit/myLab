import QtQuick 2.0
import QtQuick.Controls 2.2

Button {
    property string basePic: ""
    property string hoverPic: ""
    property string pressedPic: ""

    id: plusButton

    width: 75
    height: 75

    hoverEnabled: true

    Image {
        id: image
        source: basePic
        anchors.fill: parent
        smooth: true
    }

    onHoveredChanged: {
        image.source = hovered ? hoverPic
                               : basePic
    }

    MouseArea {
        anchors.fill: parent

        onPressed: {
            image.source = pressedPic
        }

        onReleased: {
            image.source = plusButton.hovered ? hoverPic
                                              : basePic
        }
    }

    background:
        Rectangle {
            color: "transparent"
    }
}
