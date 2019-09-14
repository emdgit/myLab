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

        MenuButton {
            id: groups

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
                plots.hovered ? chart.marker = plots.width
                              : chart.marker = 0
            }
        }

        MenuButton {
            id: users

            onReleased: {
                buttonClicked( "users" )
            }
        }
    }
}
