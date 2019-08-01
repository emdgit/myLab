import QtQuick 2.0

Image {
    id: button

    states: [

        State {
            name: "hovered"
            when: mouseArea.pressed

            PropertyChanges {
                target: button
                opacity: 1
            }
        },

        State {
            name: "normal"
            when: !mouseArea.pressed

            PropertyChanges {
                target: button
                opacity: 0.7
            }
        }

    ]

    Behavior on opacity {
        NumberAnimation {
            duration: 75
        }
    }

    MouseArea
    {
        anchors.fill: parent

        id: mouseArea

        onClicked: callback()
    }
}
