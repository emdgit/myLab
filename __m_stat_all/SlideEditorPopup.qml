import QtQuick 2.0
import QtQuick.Controls 2.0

/// Двигающийся "виджет" с лейблом слева и полем для ввода данных
/// справа.
/// При вводе текста в поле ввода появляется popup с подсказками
Rectangle {

    id: topRect

    /// Отступ
    readonly property int hMargin: 6

    /// Установка текста в лейбл (слева)
    property alias text: label.text

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
        height: heightFunc()
        border.color: "lightgray"
        radius: 15
        Behavior on width {
            NumberAnimation {
                duration: 300
            }
        }
        TextField {
            id: textField
            anchors {
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: 28
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
                    textRect.triggerHintPanel( false )
                }
            }
            onAccepted: {
                textField.focus = false
                textRect.triggerHintPanel( false )
            }
            onTextEdited: {
                if ( hintRect.hidden ) {
                    textRect.triggerHintPanel( true )
                }

                if ( text === qsTr("") ) {
                    textRect.triggerHintPanel( false )
                    return
                }

                ModelManager.hintModel.setHintFactor( text )
            }
            Keys.onEscapePressed: {
                textRect.triggerHintPanel( false )
            }
        }
        Rectangle {
            id: hintRect
            property bool hidden: true
            property alias viewOpacity: hintView.opacity
            ListView {
                id: hintView
                anchors.fill: parent
                model: ModelManager.hintModel.model
                delegate: Rectangle {
                    property string d: modelData
                    height: 30
                    width: parent.width
                    Text {
                        anchors.fill: parent
                        text: d
                        horizontalAlignment: Text.AlignHCenter
                    }
                }
            }
            anchors {
                top: textField.bottom
                left: parent.left
                right: parent.right
                leftMargin: topRect.border.width
                rightMargin: topRect.border.width
            }
            states: [
                State {
                    name: "hidden"
                    when: hintRect.hidden
                    PropertyChanges {
                        target: hintRect
                        height: 0
                        viewOpacity: 0
                    }
                },
                State {
                    name: "shown"
                    when: !hintRect.hidden
                    PropertyChanges {
                        target: hintRect
                        height: 100
                        viewOpacity: 1
                    }
                }
            ]
        }

        /// Показать/скрыть панель подсказок
        /// true - показать, false - скрыть
        function triggerHintPanel( mode ) {
            if ( hintRect.hidden !== mode ) {
                return
            }

            hintRect.hidden = !mode
        }

        /// Функция рассчета вышины для textRect
        function heightFunc() {
            if ( hintRect.hidden ) {
                return textField.height
            } else {
                return textField.height + hintRect.height + radius
            }
        }

    }
}
