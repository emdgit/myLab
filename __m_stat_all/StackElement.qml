import QtQuick 2.0

Rectangle {

    color: 'red'

    implicitHeight: 200;
    implicitWidth: 200;

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log( "#" + number + " - Click!" )
            qq()
        }
    }
}
