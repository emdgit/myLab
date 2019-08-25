import QtQuick 2.0

Item {

    id: menuBar
    z: 10
    width: 80
    height: parent.height
    anchors.left: parent.left
    anchors.top: parent.top
    anchors.bottom: parent.bottom

    signal buttonClicked ( string name )

    Column {

        spacing: 30

        MenuButton {
            id: groups
            imgSource: "qrc:/img/qqq.jpg"

            onReleased: {
                buttonClicked( "groups" )
            }
        }

        MenuButton {
            id: plots
            imgSource: "qrc:/img/graphs.jpg"

            onReleased: {
                buttonClicked( "graphs" )
            }
        }

        MenuButton {
            id: users
            imgSource: "qrc:/img/users.png"

            onReleased: {
                buttonClicked( "users" )
            }
        }

    }
}
