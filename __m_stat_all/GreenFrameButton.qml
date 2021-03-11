import QtQuick 2.0

Rectangle {
    readonly property int w: 20
    property alias image: buttonImg.source
    width: w
    height: w
    function rgba(r, g, b, a) {
        let red = r / 255;
        let green = g / 255;
        let blue = b / 255;
        return Qt.rgba(red, green, blue, a);
    }
    border {
        color: rgba(92, 184, 92, 1)
        width: 2
    }
    radius: 5
    color: rgba(92, 184, 92, 0.5)
    Image {
        id: buttonImg
        anchors.fill: parent
        anchors.margins: 4
        sourceSize {
            width: parent.width
            height: parent.height
        }
        source: "qrc:/img/images/plus.svg"
    }
}
