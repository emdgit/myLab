import QtQuick 2.0

Rectangle {

    property alias text : rectText.text
    property bool show: false
    property bool hide: false

    id: textRect

    width: parent.width
    height: parent.height

    x: -width
    y: 0


    Text {
        id: rectText
        anchors.fill: parent
        font.family: "Arial Black"
        horizontalAlignment: Qt.AlignHCenter
        verticalAlignment: Qt.AlignVCenter
    }


    states: [

        State {
            name: "hide"
            when: hide === true

            PropertyChanges {
                target: textRect
                x: textRect.parent.width
            }

            StateChangeScript {
                script: textRect.destroy( 350 )
            }
        },

        State {
            name: "show"
            when: show === true

            PropertyChanges {
                target: textRect
                x: 0
            }
        }


    ]


    Behavior on x {
        NumberAnimation {
            duration: 300
        }
    }

}
