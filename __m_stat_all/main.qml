import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import OwlComponents 1.0

Window {
    id: mainWindow
    visible: true
    width: Screen.desktopAvailableWidth
    height: Screen.desktopAvailableHeight
    title: qsTr("Uot tak uot")
//    color: "black"

    MMenuBar {
        id: bar

        onButtonClicked: {
            topLabel.setText(topLabelText(number))
            mainWindow.onButtonClicked(number)
        }

    }

    TopLabel {
        id: topLabel
        anchors.left: bar.right
    }

    StackLayout {
        id: layout

        anchors.left: bar.right
        anchors.top: topLabel.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom

        currentIndex: 0

        GroupsForm{
            id: first
        }
        StatisticForm{
            id:second
        }
        InfoForm{
            id: third
        }

    }

    Component.onCompleted: {
        topLabel.text = topLabelText(layout.currentIndex);
    }

    // Handler of menu buttons click
    function onButtonClicked( number ) {
        if ( number !== layout.currentIndex ) {
            layout.itemAt(layout.currentIndex).focus = false
            layout.currentIndex = number
        }
    }

    function topLabelText(number) {
        switch (number) {
            case 0: return qsTr("Группы и записи");
            case 1: return qsTr("Статистика и учёт");
            case 2: return qsTr("Информация. Всякая. Надо так. Пока что.");
            default: return qsTr("Кто-то налажал в main.qml");
        }
    }
}


