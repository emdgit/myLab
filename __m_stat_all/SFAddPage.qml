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

    // Активный режим. Прибыль / расход.
    property bool profit: false

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
                RowLayout {
                    id: recordLayout
                    SlideEditorPopup{
                        id: recordEditor
                        internalId: 0
                        text: qsTr("Запись")
                        /// Сигнал вызывается всякий раз, когда TextField получает фокус
                        onActivated: { switchSlideEditor(number) }
                        /// Сигнал вызывается, когда был нажат Enter при отсутствии
                        /// существующих записей
                        onNeedNewRecord: { topItem.onNeedNewRecord() }
                        onNoRecordsDetected: {
                            recordNotifier.show();
                            recordBlub.ok = false;
                        }
                        onEmptyRecord: { recordNotifier.hide(); }
                        onRecordEdited: {
                            if ( !recordNotifier.isHidden ) {
                                recordNotifier.hide()
                            }
                            recordBlub.ok = ModelManager.hintModel.containsRecord(record);
//                            console.log(text, recordBlub.ok);
                        }
                        onRecordAccepted: {
                            recordBlub.ok = true;
                            gTree.opacity = 0;
                        }
                    }
                    Bulb { id: recordBlub }
                }
                RowLayout {
                    id: summLayout
                    SlideEditor{
                        id: summEditor
                        internalId: 1
                        text: qsTr("Сумма")
                        validator: DoubleValidator {
                            bottom: 1
                            top: 2000000000
                        }
                        onEditorTextChanged: {
                            summBlub.ok = editorText !== "";
                        }
                    }
                    Bulb { id: summBlub }
                }
                RowLayout {
                    id: dateLayout
                    SlideEditor{
                        id: dateEditor
                        internalId: 2
                        text: qsTr("Дата")
                        onEditorTextChanged: {
                            dateBlub.ok = CoreAPI.checkDateFormat(editorText);
                        }
                    }
                    Bulb { id: dateBlub }
                }
                MSpinBox {
                    id: countEditor
                }
                Rectangle {
                    id: spacer1
                    height: 80
                }
                CartButton {
                    id: cartButton
                    hidden: !allBulbsAreGreen()
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        topItem.addTransaction();
                    }
                }
                Rectangle {
                    id: spacer2
                    Layout.fillHeight: true
                }
                RowLayout {
                    id: switchLine
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
                                gTree.model = ModelManager.profitModel;
                            } else {
                                gTree.model = ModelManager.spendModel;
                            }

                            CoreAPI.switchHintModel(checked);
                            gTree.profit = checked;
                            topItem.profit = checked;
                            cartButton.profit = checked;

                            recordEditor.clear();
                            recordBlub.ok = false;
                            summEditor.clear();
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
                CoreAPI.addRecord( groupId, recordEditor.record, topItem.profit );
                recordBlub.ok = true;
            }
        }

        NoRecordNotifier{
            id: recordNotifier
            x: dataFrame.x + dataFrame.width + 23
            onSelectClicked: { topItem.onNeedNewRecord() }
        }
    }

    PurchaseView {

        id: purchaseView

        anchors {
            top: topItem.top
            bottom: topItem.bottom
            right: topItem.right
            bottomMargin: 20
        }

        width: parent.width / 4 + 55
    }


    /// Обработчик сигналов активации виджетов "SlideEditor",
    /// устанавливает активному эдитору повышенное значение z
    function switchSlideEditor( activeEditor ) {
        var topZ = 42   // Потому что.
        var botZ = 0

        function setZ( recZ, sumZ, dateZ ) {
            recordLayout.z = recZ
            summLayout.z = sumZ
            dateLayout.z = dateZ
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

    function allBulbsAreGreen() {
        return recordBlub.ok && summBlub.ok && dateBlub.ok;
    }

    function addTransaction() {
        if (!allBulbsAreGreen()) {
            return;
        }

        var record = recordEditor.record;
        var summ = summEditor.editorData;
        var date = dateEditor.editorData;
        var amount = countEditor.value();

        recordEditor.clear();
        recordBlub.ok = false;
        summEditor.clear();
        countEditor.reset();

        if ( profit ) {
            CoreAPI.addProfit(record, summ, date, amount);
        } else {
            CoreAPI.addPurchase(record, summ, date, amount);
        }
    }
}
