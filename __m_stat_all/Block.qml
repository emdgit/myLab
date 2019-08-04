import QtQuick 2.0

Image {

    id: block

    property bool show: false
    property bool remove: false

    fillMode: Image.Stretch

    states: [

        State {
            name: "remove"
            when: remove == true

            PropertyChanges {
                target: block
                opacity: 0
            }

            StateChangeScript {
                script: block.destroy(1000)
            }
        },

        State {
            name: "show"
            when: show == true

            PropertyChanges {
                target: block
                opacity: 1
            }
        }
    ]

    Behavior on opacity { NumberAnimation { duration: 300 } }

}
