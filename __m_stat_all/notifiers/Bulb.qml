import QtQuick 2.0

// Красно-зеленый индикатор
Rectangle {
    color: "transparent"

    // ok - green, not ok - red =)
    property bool ok: false

    width: 10
    height: 10

    Image {
        id: bulbLight

        anchors.fill: parent

        sourceSize.width: 10
        sourceSize.height: 10

        source: ok ? "qrc:/img/images/green_circle.svg"
                   : "qrc:/img/images/red_circle.svg"
    }

//    onWidthChanged: {
//        height = width;
//    }

//    onHeightChanged: {
//        width = height;
//    }
}
