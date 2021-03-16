import QtQuick 2.0
import QtQuick.Controls 2.0

import OwlComponents 1.0

import "Common.js" as Script

/// Line editor с закругленными краями
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

    /// Если есть ошибка, текст краснеет, не вызывается 'accepted'
    property bool hasError: false

    /// Сигнал вызывается, когда нажат Enter, передает текс из эдитора
    signal accepted(string editorText)

    /// Сигнал вызывается, когда был нажат escape
    signal escaped()

    /// Сигнал вызывается, когда текст был изменен
    signal textChanged(string editorText)

    function clear() {
        textField.text = "";
    }

    function setError(err) {
        hasError = err;
    }

    height: textRect.height
    width: m_width
    color: "transparent"

    Rectangle {

        id: textRect

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: textField.height
        border.color: "#183A8F"
        color: Script.topLabelBackgroundColor()
        radius: 15

        TextField {

            id: textField

            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }

            enabled: false

            font.family: Script.menuTextFontFamily()
            font.pixelSize: 16

            verticalAlignment: Text.AlignVCenter

            background: Rectangle { color: "transparent"; }

            color: topRect.hasError ? CoreAPI.rgba(217, 65, 61, 1)
                                    : Script.menuTextColor()
            placeholderTextColor: CoreAPI.rgba(228, 238, 208, 0.5)

            onAccepted: {
                if (!topRect.hasError) {
                    textField.focus = false;
                    topRect.accepted( text );
                }
            }

            onTextChanged: {
                topRect.textChanged(text);
            }

            Keys.onEscapePressed: {
                textField.focus = false;
                topRect.escaped();
            }
        }
    }
}

