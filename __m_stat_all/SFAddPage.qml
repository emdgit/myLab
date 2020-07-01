import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0 // 1.12 Win

import "./notifiers"

import OwlComponents 1.0

import "Common.js" as Script

/// Страница добавления данных на форме статистик
/// Statistic Form Add Page
Item {

    id: topItem

    /// Фон. Подождёт рабочего функционала
    /*
    Image {
        id: name
        source: "qrc:/img/images/background.jpg"
        anchors.fill: parent
        fillMode: Image.Tile
    }
    */

    MouseArea {
        anchors.fill: parent
        onClicked: {
            recordEditor.forceActiveFocus()
            summEditor.forceActiveFocus()
            summEditor.forceActiveFocus()
        }
    }

    FocusScope{
        id: fScope

        anchors {
            horizontalCenter: parent.horizontalCenter
            top: parent.top
            bottom: parent.bottom
        }

        Frame {
            id: dataFrame
            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                bottom: parent.bottom
                bottomMargin: 20
            }
            ColumnLayout {
                anchors.fill: parent
                SlideEditorPopup{
                    id: recordEditor
                    internalId: 0
                    text: qsTr("Запись")
                    onActivated: { switchSlideEditor(number) }
                    onNeedNewRecord: { topItem.onNeedNewRecord() }
                    onNoRecordsDetected: { recordNotifier.show() }
                    onEmptyRecord: { recordNotifier.hide() }
                    onRecordEdited: {
                        if ( !recordNotifier.isHidden ) {
                            recordNotifier.hide()
                        }
                    }
                }
                SlideEditor{
                    id: summEditor
                    internalId: 1
                    text: qsTr("Сумма")
                    validator: IntValidator {
                        bottom: 1
                        top: 2000000000
                    }
                }
                SlideEditor{
                    id: dateEditor
                    internalId: 2
                    text: qsTr("Дата")
                    validator: RegExpValidator {
                        regExp: /\d{2}\.\d{2}\.\d{4}/
                    }
                }
                Rectangle {
                    Layout.fillHeight: true
                }
                RowLayout {
                    width: parent.width
                    Label {
                        text: qsTr("Расход")
                        Layout.preferredWidth: 100
                        Layout.alignment: Qt.AlignLeft
                        font.family: Script.menuTextFontFamily()
                    }
                    Switch {
                        Layout.alignment: Qt.AlignHCenter
                        Layout.preferredWidth: 100
                        onCheckedChanged: {
                            if (checked) {
                                // Доход
                                CoreAPI.switchHintModel(true)
                            } else {
                                CoreAPI.switchHintModel(false)
                            }
                        }
                    }
                    Text {
                        text: qsTr("Доход")
                        Layout.alignment: Qt.AlignRight
                        Layout.preferredWidth: 100
                        horizontalAlignment: Text.AlignRight
                        font.family: Script.menuTextFontFamily()
                    }
                }
            }
        }

        PGroupsTree {
            id: gTree
            x: dataFrame.x - width
            anchors {
                top: parent.top
                bottom: parent.bottom
                bottomMargin: 20
            }
            onAccept: {
                // todo ( real profit flag )
                CoreAPI.addRecord( groupId, recordEditor.record, false );
            }
        }

        NoRecordNotifier{
            id: recordNotifier
            x: dataFrame.x + dataFrame.width + 23
            onSelectClicked: { topItem.onNeedNewRecord() }
        }
    }


    /// Обработчик сигналов активации виджетов "SlideEditor",
    /// устанавливает активному эдитору повышенное значение z
    function switchSlideEditor( activeEditor ) {
        var topZ = 42   // Потому что.
        var botZ = 0

        function setZ( recZ, sumZ, dateZ ) {
            recordEditor.z = recZ
            summEditor.z = sumZ
            dateEditor.z = dateZ
        }

        switch ( activeEditor ) {
            case 0:
                setZ( topZ, botZ, botZ )
                break
        }
    }

    /// Открывает просмотр групп при добавлении новой записи
    function showGroups() {
        gTree.opacity = 1
    }

    /// Обработчик сигнала needNewRecord, поступившего с эдитора
    function onNeedNewRecord() {
        showGroups()

        if ( !recordNotifier.isHidden ) {
            recordNotifier.hide()
        }
    }
}
