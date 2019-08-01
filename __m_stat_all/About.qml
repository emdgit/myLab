import QtQuick 2.0

Rectangle {

    id: about
    opacity: 0
    color: "transparent"
    anchors.fill: parent

    function show() {
        about.opacity = 1;
    }

    function hide() {
        about.opacity = 0;
    }

    Common {
        id: cData
    }

    Rectangle {

        id: aboutMask
        color: "gray"
        opacity: 0.5
        anchors.fill: parent
    }

    Rectangle {

        id: dialog
        width: 360
        height: 230
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        z: 10
        border.color: "gray"

        Text {
            id: aboutText
            text: cData._aboutText
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    Behavior on opacity
    {
        NumberAnimation { duration: 100 }
    }

    MouseArea
    {
        anchors.fill: parent;

        onClicked: hide();
    }
}
