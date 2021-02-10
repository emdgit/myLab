import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.1

import OwlComponents 1.0

// Form which opens when statistic button pressed
Item {

    Button {
        id: titleMenuBtn
        height: 30
        width: 30

        anchors {
            top: parent.top
            left: parent.left
        }

        palette {
            button: "transparent"
        }

        Image {
            anchors.fill: parent
            source: "qrc:/img/images/lined_menu.svg"
        }

        onClicked: {
            parent.showTopMenu()
        }
    }

    TopStatisticMenu {
        anchors {
            top: parent.top
            left: titleMenuBtn.right
            rightMargin: 9
        }

        onButtonClicked: {
            swipeTo(number)
        }
    }

    SwipeView {
        id: swipeView
        orientation: Qt.Horizontal
        currentIndex: 1

        anchors {
            top: titleMenuBtn.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: 9
        }

        Item {
            id: page1
            GeneralStatisticsPage { anchors.fill: parent }
        }
        Item {
            id: page2
            AddPurchasePage {
                id: widget
                anchors.fill: parent
            }
        }
    }

    /// Показать меню статистик
    function showTopMenu() {
    }

    /// Обработчик нажатия кнопок верхнего меню статистик.
    /// Вертит SwipeView
    function swipeTo(index) {
        swipeView.currentIndex = index
    }
}
