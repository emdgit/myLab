import QtQuick 2.0
import QtQuick.Layouts 1.1

import "Common.js" as Script

Rectangle {

    id: topRect

    property int m_width: 300

    width: m_width
    height: 40

    QtObject {
        id: _d
        property int value: 1
    }

    RowLayout {
        anchors.fill: parent

        Rectangle {
            id: spacer
            height: parent.height
            Layout.fillWidth: true
        }

        PicButton {
            id: downButton

            radius: height / 2

            enabled: false

            defaultPic: "qrc:/img/images/arrow_down.svg"
            hoverPic: "qrc:/img/images/arrow_down_hovered.svg"
            unactivePic: "qrc:/img/images/arrow_down_unactive.svg"

            onClicked: {
                topRect.onPicButtonClicked(-1);
            }
        }

        Rectangle {
            id: valueRect

            height: 40
            width: 40

            Text {
                anchors.fill: parent
                text: _d.value
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: Script.menuTextFontFamily()
                font.pixelSize: Script.menuTextFontSize() - 2
                color: Script.orangeColor()
            }
        }

        PicButton {
            id: upButton

            radius: height / 2

            // Потому что у Спин-боксов в SFAddPage такой же отступ
            Layout.rightMargin: 6

            defaultPic: "qrc:/img/images/arrow_up.svg"
            hoverPic: "qrc:/img/images/arrow_up_hovered.svg"
            unactivePic: "qrc:/img/images/arrow_up_unactive.svg"

            onClicked: {
                topRect.onPicButtonClicked(1);
            }
        }
    }

    // Вернуть текущее значение
    function value() {
        return _d.value
    }

    // Обработчик нажатия кнопок ( -1 / 1 )
    function onPicButtonClicked( val ) {
        _d.value = _d.value + val;

        if ( _d.value === 1 ) {
            downButton.enabled = false;
        } else if ( downButton.enabled === false ) {
            downButton.enabled = true;
        }
    }
}
