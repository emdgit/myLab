import QtQuick 2.0

/// Кнопка переключения режима на доход/расход
Item {

    id: topItem

    readonly property int w: 35
    readonly property int duration: 100
    property bool isProfit: false

    width: w
    height: w - 7

    onIsProfitChanged: {
        if (isProfit) {
            imageProfit.opacity = 1;
            imageSpend.opacity = 0;
        } else {
            imageProfit.opacity = 0;
            imageSpend.opacity = 1;
        }
    }

    Image {
        id: imageSpend
        anchors.fill: parent
        sourceSize {
            width: topItem.w
            height: topItem.w
        }
        source: "qrc:/img/images/spend.svg"
        opacity: 1
        Behavior on opacity {
            NumberAnimation { duration: topItem.duration }
        }
    }

    Image {
        id: imageProfit
        anchors.fill: parent
        sourceSize {
            width: topItem.w
            height: topItem.w
        }
        source: "qrc:/img/images/profit.svg"
        opacity: 0
        Behavior on opacity {
            NumberAnimation { duration: topItem.duration }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onClicked: {
            topItem.isProfit = !topItem.isProfit;
        }
        onEntered: {
            cursorShape = Qt.PointingHandCursor;
        }
        onExited: {
            cursorShape = Qt.ArrowCursor;
        }
    }
}
