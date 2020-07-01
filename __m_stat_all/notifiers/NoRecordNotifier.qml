import QtQuick 2.0
import QtQuick.Controls 2.0

import "../Common.js" as Script

/// Уведомление о том, что вводимая пользователем
/// запись не обнаружена. Предложение добавить её.
Rectangle {

    id: topRect

    readonly property int hMargin: 6
    readonly property int m_width: 178

    property bool isHidden: true

    /// Нажатие на кнопку "Выбрать"
    signal selectClicked()

    radius: 10
    height: 70
    width: 0
    opacity: 0

    color: Script.hoveredColor()
    Text {
        id: label
        text: qsTr("Записей не обнаружено")
        height: 30
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        color: Script.menuTextColor()
        font.family: Script.menuTextFontFamily()
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
    }

    Button {
        width: parent.width * 0.85
        height: 25
        text: qsTr("Выбрать")
        anchors {
            top: label.bottom
            topMargin: 5
            horizontalCenter: parent.horizontalCenter
        }
        background: Rectangle {
            color: "green"
            radius: height / 2
        }
        onClicked: {
            selectClicked()
            hide()
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }

    Behavior on rotation {
        NumberAnimation {
            duration: 300
        }
    }

    Behavior on width {
        NumberAnimation {
            duration: 300
        }
    }

    function show() {
        isHidden = false
        topRect.opacity = 1
        topRect.rotation = 360
        topRect.width = m_width
    }

    function hide() {
        isHidden = true
        topRect.opacity = 0
        topRect.rotation = 0
        topRect.width = 0
    }
}
