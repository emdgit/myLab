import QtQuick 2.0
import QtQuick.Controls 2.0

import "Common.js" as Script

/// Двигающийся "виджет" с лейблом слева и полем для ввода данных
/// справа.
/// При вводе текста в поле ввода появляется popup с подсказками
Rectangle {

    id: topRect

    /// Отступ
    readonly property int hMargin: 6

    /// Установка текста в лейбл (слева)
    property alias text: label.text

    /// Текст из эдитора
    property alias record: textField.text

    /// Ширина виджета
    property int m_width: 300

    /// Внутренний идентификатор
    property int internalId: 0

    /// Сигнал вызывается всякий раз, когда TextField получает фокус
    signal activated( int number )

    /// Сигнал вызывается, когда был нажат Enter при отсутствии
    /// существующих записей
    signal needNewRecord( string recordName )

    /// Сигнал вызывается всякий раз, когда не были обнаружены
    /// записи, содержащие в своем названии введенный в эдитор текст
    signal noRecordsDetected()

    /// Сигнал вызывается всякий раз, когда в процессе редактирования
    /// записи, весь текст её названия оказался стерт
    signal emptyRecord()

    /// Сигнал вызывается всякий раз, когда имя записи редактируется
    /// и не является пустым
    signal recordEdited( string currentName )

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
            height: 28
            font.family: Script.menuTextFontFamily()
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
                    textRect.triggerHintPanel( false )
                }
            }
            onAccepted: {
                if ( hintRect.hidden ) {
                    var x = ModelManager.hintModel.containsRecord( text );
                    if ( x === false ) {
                        /// Создать новую запись
                        topRect.needNewRecord( text );
                        console.log("Need new. Show groups!");
                    }
                } else {
                    /// Взять выбранную из подсказок запись
                    text = hintRect.currentHint;
                }

                textField.focus = false;
                textRect.triggerHintPanel( false );
            }
            onTextEdited: {
                if ( hintRect.hidden ) {
                    textRect.triggerHintPanel( true )
                }

                if ( text === qsTr("") ) {
                    textRect.triggerHintPanel( false )
                    emptyRecord()
                    return
                }

                ModelManager.hintModel.setHintFactor( text )
                recordEdited( text )

                if ( !hintRect.count ) {
                    textRect.triggerHintPanel(false)
                    noRecordsDetected()
                }
            }
            Keys.onEscapePressed: {
                textRect.triggerHintPanel( false )
                textField.focus = false
            }
            Keys.onDownPressed: {
                textRect.onDownPressed()
            }
            Keys.onUpPressed: {
                textRect.onUpPressed()
            }
        }
        Rectangle {
            id: hintRect
            property alias viewOpacity: hintView.opacity
            property alias count: hintView.count
            property alias currentIndex: hintView.currentIndex

            property string currentHint : ""
            property bool hidden: true

            ListView {
                id: hintView
                anchors.fill: parent
                model: ModelManager.hintModel.model
                onModelChanged: {
                    if ( count === 0 ) {
                        // Подсказок нет
                        currentIndex = -1
                        textRect.triggerHintPanel(false)
                    } else {
                        if ( count <= currentIndex ) {
                            currentIndex = 0
                        }
                    }
                }
                delegate: Rectangle {
                    property string d: modelData
                    height: 30
                    color: {
                        if ( index === hintRect.currentIndex ) {
                            hintRect.currentHint = d
                            return Script.defaultUnhoveredColor()
                        }

                        return Script.hoveredColor()
                    }
                    width: parent.width

                    Text {
                        anchors.fill: parent
                        text: d
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: Script.menuTextColor()
                        font.family: Script.menuTextFontFamily()
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
                        currentIndex: -1
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

        /// Обработчик нажатия кнопки "вниз", при вводе записи в textField
        function onDownPressed() {
            if ( !hintRect.count ) {
                return
            }

            if ( hintRect.currentIndex === hintRect.count - 1 ) {
                return
            }

            hintRect.currentIndex++
        }

        /// Обработчик нажатия кнопки "вверх", при вводе записи в textField
        function onUpPressed() {
            if ( !hintRect.count || hintRect.currentIndex <= 0 ) {
                return
            }

            hintRect.currentIndex--;
        }
    }
}

