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

    MMenuBar {
        id: bar

        onButtonClicked: {
            topLabel.setText( name )
            mainWindow.onButtonClicked( number )
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

        currentIndex: 1

        StackElement{
            id: first
        }
        StatisticForm{
            id:second
        }
        StackElement{
            id: third
            color: 'blue'
        }

    }

    // Handler of menu buttons click
    function onButtonClicked( number ) {
        if ( number !== layout.currentIndex ) {
            layout.itemAt(layout.currentIndex).focus = false
            layout.currentIndex = number
        }
    }

}


