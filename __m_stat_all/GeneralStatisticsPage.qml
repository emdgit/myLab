import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

import "Common.js" as Script

Item {

    id: topItem

    Rectangle {

        id: gradientRect

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        height: 40

        LinearGradient {
            id: linearGR
            anchors.fill: parent
            start: Qt.point(0,0)
            end: Qt.point(width,0)
            gradient: Gradient {
                GradientStop { position: 0.0; color: Script.defaultUnhoveredColor() }
                GradientStop {
                    position: pos()
                    color: Script.hoveredColor()
                    function pos() {
                        if (!mArea.containsMouse)
                            return 0.5;
                        else {
                            return (mArea.mouseX / linearGR.width);
                        }
                    }
                }
                GradientStop { position: 1.0; color: Script.defaultUnhoveredColor() }
            }
        }

        RowLayout {
            anchors.fill: parent
            Item {
                height: parent.height
                Text {
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    verticalAlignment: Text.AlignVCenter
                    text: qsTr("Доход: 57394₽")
                    color: Script.menuTextColor()
                    font.family: Script.menuTextFontFamily()
                    font.pixelSize: Script.menuTextFontSize() - 2
                }
//                Rectangle {
//                    width: 100
//                    height: parent.height
//                    anchors.verticalCenter: parent.verticalCenter
//                    color: "red"
//                }
            }
        }

        MouseArea {
            id: mArea
            anchors.fill: parent
            hoverEnabled: true
        }
    }

}
