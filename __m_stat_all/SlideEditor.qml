import QtQuick 2.0
import QtQuick.Controls 2.0

import "Common.js" as Script

/// Двигающийся "виджет" с лейблом слева и полем для ввода данных
/// справа.
Rectangle {

    id: topRect

    /// Отступ
    readonly property int hMargin: 6

    /// Установка текста в лейбл (слева)
    property alias text: label.text

    property alias validator: textField.validator

    /// Ширина виджета
    property int m_width: 300

    /// Внутренний идентификатор
    property int internalId: 0

    /// Сигнал вызывается всякий раз, когда TextField получает фокус
    signal activated( int number )

    height: 40
    width: m_width

    Label {
        id: label

        x: hMargin
        y: (topRect.height / 2) - ( height / 2 )

        width: topRect.width * 0.25 - hMargin

        text: qsTr(topRect.text)

        Behavior on width {
            NumberAnimation {
                duration: 300
            }
        }

        Behavior on opacity {
            NumberAnimation {
                duration: 200
            }
        }
    }

    Rectangle {

        id: textRect

        x: hMargin + label.width
        y: hMargin

        width: topRect.width - label.width - 2 * hMargin
        height: textField.height

        border.color: "lightgray"
        radius: 15

        Behavior on width {
            NumberAnimation {
                duration: 300
            }
        }

        TextField {

            id: textField

            height: 28

            font.family: Script.menuTextFontFamily()

            validator: topRect.m_validator

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            background: Rectangle {
                color: "transparent"
            }

            onFocusChanged: {
                if (focus) {
                    label.width = 0
                    label.opacity = 0
                    textRect.width = topRect.width - 2*hMargin
                    textField.placeholderText = label.text + "..."
                    topRect.activated(topRect.internalId)
                } else {
                    label.width = topRect.width * 0.25 - hMargin
                    label.opacity = 1
                    textRect.width = topRect.width * 0.75 - hMargin
                    textField.placeholderText = ""
                }
            }

            onAccepted: {
                textField.focus = false
            }

            Keys.onEscapePressed: {
                textField.focus = false
            }
        }
    }
}

