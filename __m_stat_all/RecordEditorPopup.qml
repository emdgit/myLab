import QtQuick 2.0
import QtQuick.Controls 2.0

import "Common.js" as Script

/// Поле с закругленными краями для ввода записи с
/// выпадающими подсказками.
Rectangle {

    id: topRect

    /// Текст из эдитора по умолчанию
    property alias placeholderText: textField.placeholderText

    /// Ширина виджета
    property int m_width: 300

    /// Внутренний идентификатор
    property int internalId: 0

    /// Корректность введенных значений.
    property bool isComplete: false


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

    /// Сигнал вызывается, когда имеется введенная запись
    signal recordAccepted()

    height: 40
    width: m_width

    Rectangle {
        id: textRect

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: heightFunc()
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
            font.family: Script.menuTextFontFamily()
            background: Rectangle {
                color: "transparent"
            }
            onAccepted: {
                textRect.onRecordAccepted();
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

            property bool hidden: true
            property int currentIndex: -1

            function getCurrentHint() {
                if (currentIndex === -1) {
                    return "";
                }
                return hintView.itemAtIndex(currentIndex).d;
            }

            ListView {
                property bool hasHover: false
                id: hintView
                anchors.fill: parent
                clip: true
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
                    property bool hovered: false

                    height: 30
                    color: isHovered() ? Script.hintHoveredColor()
                                       : Script.hintUnhoveredColor()
                    width: topRect.m_width

                    Text {
                        anchors.fill: parent
                        text: d
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        color: parent.isHovered() ? Script.hintTextColorHovered()
                                                  : Script.hintTextColorUnhovered()
                        font.family: Script.menuTextFontFamily()
                        font.bold: parent.isHovered() ? true : false
                    }

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered: {
                            parent.hovered = true;
                            hintView.hasHover = true;
                        }
                        onExited: {
                            parent.hovered = false;
                            hintView.hasHover = false;
                        }
                        onClicked: {
                            hintRect.currentIndex = parent.getIndex();
                            textRect.onRecordAccepted();
                        }
                    }

                    function isHovered() {
                        if (hovered) {
                            return true;
                        } else if (hintView.hasHover === true) {
                            return false;
                        } else if (index === hintRect.currentIndex) {
                            return true;
                        }
                        return false;
                    }

                    function getIndex() {
                        return index;
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
                        height: 400
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
            if ( !hintRect.count || hintRect.currentIndex < 0 ) {
                return
            }

            hintRect.currentIndex--;
        }

        /// Обработчик нажатия "Enter", при вводе записи в textField
        /// Или запись выбрана мышью из выпадающих подсказок
        function onRecordAccepted() {
            function checkIfNeedNewRecord() {
                if (!ModelManager.hintModel.containsRecord(textField.text)) {
                    /// Нужно создать новую запись
                    topRect.needNewRecord(textField.text);
                } else {
                    topRect.recordAccepted();
                }
                textField.focus = false;
                textRect.triggerHintPanel( false );
            }

            if (hintRect.hidden) {
                checkIfNeedNewRecord();
                return;
            }

            if (hintRect.currentIndex < 0) {
                checkIfNeedNewRecord();
                return;
            }

            /// Взять выбранную из подсказок запись
            textField.text = hintRect.getCurrentHint();
            textField.focus = false;

            textRect.triggerHintPanel(false);

            topRect.recordAccepted();
        }
    }

    function clear() {
        textField.text = "";
    }
}

