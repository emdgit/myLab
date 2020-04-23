import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.12

import OwlComponents 1.0

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("Uot tak uot")

    MenuBar {
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

        currentIndex: 0

        StackElement{
            id: first
        }
        StatisticMenu{
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


