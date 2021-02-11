import QtQuick 2.0

import "Common.js" as Script

/// Верхний виджет, рисующий название текущей формы.
Item {

    id: topLabel

    property alias text: textRect.text

    anchors.top: parent.top
    anchors.right: parent.right

    height: 70

    TextRect {

        id: textRect
        text: "sdf"
        show: true
    }


    function setText( text ) {
        Script.removeOldTextRects()
        Script.createNewTextRect( text )
    }

}
