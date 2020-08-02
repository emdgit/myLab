import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0 // 1.12 Win
import QtQuick.Layouts 1.3

import "Common.js" as Script

Item {

    id: topItem

    StackLayout {
        id: stack
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: switchLine.top
        }

        currentIndex: 0

        ListView {
            id: list
            model: ModelManager.purchaseModelDaily.days()

            delegate: Item {
                property int row: index
                property int count: ModelManager.purchaseModelDaily.count(row)
                width: parent.width
                height: 30
                Text {
                    anchors.fill: parent
                    text: index
                }
            }
        }

        TableView {
            id: table
            model: ModelManager.purchaseModel
            TableViewColumn {
                role: "PName"
                title: qsTr("Запись")
            }
            TableViewColumn {
                role: "PSumm"
                title: qsTr("Сумма")
            }
            TableViewColumn {
                role: "PLastDate"
                title: qsTr("Последняя покупка")
            }
        }


    }


    RowLayout {
        id: switchLine
        height: 50
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        Label {
            text: qsTr("Дневной")
            Layout.preferredWidth: 100
            Layout.alignment: Qt.AlignLeft
            Layout.leftMargin: 10
            font.family: Script.menuTextFontFamily()
        }
        Switch {
            Layout.alignment: Qt.AlignHCenter
            Layout.preferredWidth: 100
            onCheckedChanged: {
                if (checked) {
                    //daily
                    stack.currentIndex = 1
                } else {
                    stack.currentIndex = 0
                }
            }
        }
        Text {
            text: qsTr("Общий")
            Layout.alignment: Qt.AlignRight
            Layout.preferredWidth: 100
            Layout.rightMargin: 10
            horizontalAlignment: Text.AlignRight
            font.family: Script.menuTextFontFamily()
        }
    }
}
