import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.0 // 1.12 Win
import QtQuick.Layouts 1.3

import OwlComponents 1.0

import "Common.js" as Script

Item {

    id: topItem

    PeriodSelector {
        id: periodSelector
        periodNumber: ModelManager.periodModel.size() - 1
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        onPeriodChanged: {
            CoreAPI.setPurchaseViewPeriod(periodNumber)
        }
    }

    StackLayout {
        id: stack
        anchors {
            top: periodSelector.bottom
            left: parent.left
            right: parent.right
            bottom: switchLine.top
        }

        currentIndex: 0

        Connections {
            target: SignalManager
            function onDailyModelReloaded() {
                list.updateModel();
            }
        }

        ListView {
            id: list

            property int days: setupModel()

            clip: true
            model: days
            delegate: Item {
                id: listDelegate
                property int row: index
                property int count: ModelManager.purchaseModelDaily.count(row)

                width: parent.width
                height: 20 * count

                Rectangle {
                    id: dayRect
                    anchors.fill: parent

                    property int ind: listDelegate.row

                    Column {
                        id: dayColumn
                        anchors.fill: parent
                        Repeater {
                            // to separate... Make Repeater
                            // or all delegate separate element.
                            model: listDelegate.count
                            Rectangle {
                                height: 20
                                width: parent.width
                                Text {
                                    id: recordNameText
                                    anchors {
                                        top: parent.top
                                        bottom: parent.bottom
                                        left: parent.left
                                    }
                                    width: parent.width * 0.45
                                    text: dayRect.getName(dayRect.ind, index)
                                    verticalAlignment: Text.AlignVCenter
                                }
                                Text {
                                    id: recordSummText
                                    anchors {
                                        top: parent.top
                                        bottom: parent.bottom
                                        right: parent.right
                                        left: recordNameText.right
                                    }
                                    color: dayRect.getSummColor(dayRect.ind, index)
                                    font.bold: dayRect.getIsProfit(dayRect.ind, index)
                                    text: dayRect.getSumm(dayRect.ind, index)
                                    verticalAlignment: Text.AlignVCenter
                                }
                            }
                        }
                    }

                    function getName(day, index) {
                        return ModelManager.purchaseModelDaily.name(day, index);
                    }

                    function getSumm(day, index) {
                        return ModelManager.purchaseModelDaily.summ(day, index);
                    }

                    function getIsProfit(day, index) {
                        return ModelManager.purchaseModelDaily.isProfit(day, index);
                    }

                    function getSummColor(day, index) {
                        var profit = getIsProfit(day, index);
                        return profit ? "green" : "red";
                    }
                }
                Rectangle {
                    id: line
                    color: "gray"
                    height: 1
                    width: parent.width
                    anchors.top: parent.top
                }
                Text {
                    id: date
                    anchors.top: line.bottom
                    anchors.right: parent.right
                    anchors.rightMargin: 20
                    text: ModelManager.purchaseModelDaily.date(row)
                }
            }

            function setupModel() {
                return ModelManager.purchaseModelDaily.days();
            }

            function updateModel() {
                var d = ModelManager.purchaseModelDaily.days();
                if (d === days) {
                    days = 0;
                }
                days = d;
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
