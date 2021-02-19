import QtQuick 2.0

import "Common.js" as Script

Item {
    id: topItem

    Rectangle {
        id: blackRect
        anchors.fill: parent
        color: "black"
    }

    Rectangle {
        id: groupRect

        width: parent.width * 0.25

        color: "transparent"

        anchors {
            left: parent.left
            leftMargin: 10
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
        }

//        border.color: Script.defaultUnhoveredColor()

        PGroupsTree {
            id: groupTree
            anchors.fill: parent
        }
    }

    Rectangle {
        id: recordRect

        width: parent.width * 0.25

        // need later
//        color: "transparent"

        anchors {
            left: groupRect.right
            leftMargin: 10
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
        }

//        border.color: Script.defaultUnhoveredColor()
    }
}


