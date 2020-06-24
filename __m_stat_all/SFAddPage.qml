import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0 // 1.12 Win

/// Страница добавления данных на форме статистик
Item {
    Frame {
        anchors.horizontalCenter: parent.horizontalCenter
        ColumnLayout {
            RowLayout {
                id: recordLay
                spacing: 6
                Label {
                    Layout.preferredWidth: 75
                    text: qsTr("Запись:")
                }
                TextField {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    placeholderText: qsTr("Название...")

                }
            }
            RowLayout {
                id: summLay
                spacing: 6
                Label {
                    Layout.preferredWidth: 75
                    text: qsTr("Сумма:")
                }
                TextField {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    placeholderText: qsTr("Сумма...")
                    validator: IntValidator {
                        bottom: 1
                        top: 2000000000
                    }
                }
            }
            RowLayout {
                id: dateLay
                spacing: 6
                Label {
                    Layout.preferredWidth: 75
                    text: qsTr("Дата:")
                }
                TextField {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 30
                    placeholderText: qsTr("dd.mm.yyyy")
                    validator: RegExpValidator {
                        regExp: /\d{2}\.\d{2}\.\d{4}/
                    }
                }
            }
            RowLayout {
                id: emptyLay
                Layout.preferredHeight: 70
            }
            RowLayout {
                id: switchLay
                spacing: 6
                Label {
                    Layout.preferredWidth: 75
                    text: qsTr("Доход")
                }
                Switch{ Layout.fillWidth: true }
                Text {
                    text: qsTr("Расход")
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 75
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
    }
}
