import QtQuick 2.0

import OwlComponents 1.0

Rectangle {

    id:             menuBar

    z:              10
    width:          80
    height:         parent.height

    color:          "#2A2440"

    anchors.left:   parent.left
    anchors.top:    parent.top
    anchors.bottom: parent.bottom

    signal buttonClicked ( string name )

    Column {

        id: column

        MenuButton {
            id: groups

            Text {
                id: groupsText

                text: qsTr("Groups")

                font.family:    "Franklin Gothic Medium Cond"
                font.pixelSize: 22

                color: "#F2EEDC"

                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
            }

            onReleased: {
                buttonClicked( "groups" )
            }
        }

        MenuButton {
            id: plots

            onReleased: {
                buttonClicked( "graphs" )
            }

            Chart {
                id: chart
                x: 0
                y: 0
                pWidth: 2
                color: "red"
                width: parent.width
                height: parent.height
                marker: 0

                Behavior on marker {
                    NumberAnimation {
                        duration: 300
                    }
                }
            }

            onHoveredChanged: {
                column.onGraphHovered( plots.hovered )
            }
        }

        MenuButton {
            id: users

            Text {
                id: usersText

                text: qsTr("Users")

                font.family:    "Franklin Gothic Medium Cond"
                font.pixelSize: 22

                color: "#F2EEDC"

                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
            }

            onReleased: {
                buttonClicked( "users" )
            }
        }

        function onGraphHovered( hovered ) {

            if ( hovered === true ) {
                chart.makeNewGraphs()
                chart.marker = plots.width
            } else {
                chart.marker = 0
            }
        }// onGraphHovered
    }// Column
}
