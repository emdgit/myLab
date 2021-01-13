import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0
import QtCharts 2.3

import OwlComponents 1.0

import "Common.js" as Script

// Самая левая страница на форме статистик
Item {

    id: topItem

    Rectangle {

        id: gradientRect

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: 40

        LinearGradient {
            id: linearGR
            anchors.fill: parent
            start: Qt.point(0,0)
            end: Qt.point(width,0)
            gradient: Gradient {
                GradientStop { position: 0.0; color: Script.defaultUnhoveredColor() }
                GradientStop {
                    position: pos()
                    color: Script.hoveredColor()
                    function pos() {
                        if (!mArea.containsMouse)
                            return 0.5;
                        else {
                            return (mArea.mouseX / linearGR.width);
                        }
                    }
                }
                GradientStop { position: 1.0; color: Script.defaultUnhoveredColor() }
            }
        }

        RowLayout {
            id: headerLayout
            anchors.fill: parent

            property int profitMoney: CoreAPI.currentProfit()
            property int spendModey: CoreAPI.currentConsuption()
            property int cleanProfit: CoreAPI.getCleanProfit()

            property real savedPercent: CoreAPI.getCleanPercent()

            Connections {
                target: SignalManager
                function onLastPeriodPurchaseAdded() {
                    headerLayout.profitMoney = CoreAPI.currentProfit();
                    headerLayout.spendModey = CoreAPI.currentConsuption();
                }
                function onPurchaseAdded() {
                    headerLayout.cleanProfit = CoreAPI.getCleanProfit();
                    headerLayout.savedPercent = CoreAPI.getCleanPercent();
                }
            }

            MoneyLabel {
                summ: parent.profitMoney
                meaning: qsTr("Доход")
            }

            MoneyLabel {
                summ: parent.spendModey
                meaning: qsTr("Расход")
            }

            MoneyLabel {
                summ: parent.profitMoney - parent.spendModey
                meaning: qsTr("За месяц")
            }

            MoneyLabel {
                summ: parent.cleanProfit
                meaning: qsTr("Итого")
            }

            MoneyLabel {
                id: percentLabel
                summ: parent.savedPercent
                meaning: qsTr("Сохранено")
            }

            QtObject {
                id: _d
                readonly property bool percentInit: percentLabel.setPercentMode()
            }
        }

        MouseArea {
            id: mArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

    Rectangle {
        id: chartProfitRect
        height: 300

        anchors {
            top: gradientRect.bottom
            topMargin: 15
            left: parent.left
            leftMargin: 15
            right: parent.right
            rightMargin: 15
            verticalCenter: parent.verticalCenter
        }

        border.color: "black"
        border.width: 2

        ChartView {
            id: chartProfit

            antialiasing: true

            anchors.fill: parent

            Component.onCompleted: {
                ChartManager.registerMain(chartProfit)
            }
        }
    }

}
