import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 2.2

import OwlComponents 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Uot tak uot")

    MenuBar {
        id: bar

        onButtonClicked: {
            topLabel.setText( name )
        }
    }

    TopLabel {
        id: topLabel
        anchors.left: bar.right
    }

    Rectangle {

        x: 300
        y: 300
        width: 300
        height: 300
    }
}


