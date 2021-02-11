import QtQuick 2.0

import OwlComponents 1.0

import "Common.js" as Script

Rectangle {

    id:             menuBar

    z:              10
    width:          80
    height:         parent.height

    color:          Script.defaultUnhoveredColor()

    anchors.left:   parent.left
    anchors.top:    parent.top
    anchors.bottom: parent.bottom

    signal buttonClicked(int number)

    Column {

        id: column

        MenuButton {
            id: groups
            number: 0

            Text {
                id: groupsText

                text: qsTr("Groups")

                font.family:    Script.menuTextFontFamily()
                font.pixelSize: Script.menuTextFontSize()

                color: Script.menuTextColor()

                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
            }

            onReleased: {
                buttonClicked(number)
            }
        }

        MenuButton {
            id: plots
            number: 1

            onReleased: {
                buttonClicked(number)
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
            id: info
            number: 2

            Text {
                id: infoText

                text: qsTr("Info")

                font.family:    "Franklin Gothic Medium Cond"
                font.pixelSize: 22

                color: Script.menuTextColor()

                anchors.fill: parent
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment:   Text.AlignVCenter
            }

            onReleased: {
                buttonClicked(number)
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
