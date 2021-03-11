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

        GroupListHeader {
            id: groupHeader
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
        }

        PGroupsTree {
            id: groupTree
            anchors {
                top: groupHeader.bottom
                topMargin: 2
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

    Rectangle {
        id: recordRect

        width: parent.width * 0.25
        color: "transparent"

        anchors {
            left: groupRect.right
            leftMargin: 10
            top: parent.top
            topMargin: 10
            bottom: parent.bottom
        }

        Connections {
            target: groupTree
            function onAccepted(id) {
                recordList.setGroupId(id);
            }
        }

        RecordListHeader {
            id: recordHeader
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
        }

        RecordList {
            id: recordList
            anchors {
                top: recordHeader.bottom
                topMargin: 2
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }
        }
    }
}


