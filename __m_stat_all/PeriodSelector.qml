import QtQuick 2.0

// Виджет выбора периода.
// Выбор стрелками влево вправо
Item {
    id: topItem

    property int periodNumber: 0

    signal periodChanged(int period)

    height: 40

    PicButton {
        id: leftButton

        anchors {
            left: parent.left
            verticalCenter: parent.verticalCenter
        }

        defaultPic: "qrc:/img/images/left_circle_arrow.svg"
        hoverPic: "qrc:/img/images/left_circle_arrow_hover.svg"
        unactivePic: "qrc:/img/images/left_circle_arrow_unactive.svg"

        onClicked: {
            periodNumber--;
            topItem.periodChanged(periodNumber);
        }
    }

    Text {
        id: periodText
        anchors {
            left: leftButton.right
            right: rightButton.left
            top: parent.top
            bottom: parent.bottom
        }
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    PicButton {
        id: rightButton

        anchors {
            right: parent.right
            verticalCenter: parent.verticalCenter
        }

        defaultPic: "qrc:/img/images/right_circle_arrow.svg"
        hoverPic: "qrc:/img/images/right_circle_arrow_hover.svg"
        unactivePic: "qrc:/img/images/right_circle_arrow_unactive.svg"

        onClicked: {
            periodNumber++;
            topItem.periodChanged(periodNumber);
        }
    }

    onPeriodNumberChanged: {
        var periods = ModelManager.periodModel.size();
        rightButton.enabled = periodNumber !== periods - 1;
        leftButton.enabled = periodNumber !== 0;
        periodText.text =
                ModelManager.periodModel.periodString(periodNumber);
    }
}
