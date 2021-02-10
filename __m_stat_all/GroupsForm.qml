import QtQuick 2.0

import "Common.js" as Script

Item {
    id: topItem

    Rectangle {
        id: groupRect

        width: parent.width * 0.25

        anchors {
            left: parent.left
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
        }

        border.color: Script.defaultUnhoveredColor()

        PGroupsTree {
            id: groupTree
            anchors.fill: parent
            opacity: 1
        }
    }

    Rectangle {
        id: recordRect

        width: parent.width * 0.25

        anchors {
            left: groupRect.right
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
        }

        border.color: Script.defaultUnhoveredColor()
    }
}


