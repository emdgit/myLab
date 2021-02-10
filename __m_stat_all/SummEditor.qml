import QtQuick 2.0
import QtQuick.Controls 2.0

import "Common.js" as Script

/// Поле для ввода суммы записи.
Rectangle {

    id: topRect

    /// Доступность виджета
    property alias editEnabled: textField.enabled

    /// Валидатор введенного значения
    property alias validator: textField.validator

    /// Placeholder
    property alias placeholderText: textField.placeholderText

    /// Текст эдитора
    property alias editorData: textField.text

    /// Ширина виджета
    property int m_width: 300

    /// Сигнал вызывается, когда нажат Enter, передает текс из эдитора
    signal accepted( string editorText )

    height: 40
    width: m_width

    Rectangle {

        id: textRect

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: textField.height
        border.color: "lightgray"
        radius: 15

        TextField {

            id: textField

            height: 28
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            enabled: false

            font.family: Script.menuTextFontFamily()

            background: Rectangle {
                color: "transparent"
            }

            onAccepted: {
                textField.focus = false;
                topRect.accepted( text );
            }

            Keys.onEscapePressed: {
                textField.focus = false
            }
        }
    }

    function clear() {
        textField.text = "";
    }
}

