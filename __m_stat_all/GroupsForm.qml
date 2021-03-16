import QtQuick 2.0

import OwlComponents 1.0
import enums 1.0

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

        function isProfit() {
            return groupHeader.isProfit;
        }

        GroupListHeader {
            id: groupHeader
            anchors {
                left: parent.left
                right: parent.right
                top: parent.top
            }
            onNewGroupClicked: {
                interactiveRect.setMode(Enums.NewGroup);
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

    Rectangle {
        id: interactiveRect

        property int mode: Enums.None

        function setMode(m) {
            if (m === mode) {
                return;
            }

            if (m === Enums.None) {
                loader.sourceComponent = noneCmp;
            } else if (m === Enums.NewGroup) {
                loader.sourceComponent = newGroupCmp;
            }
            mode = m;
        }

        anchors {
            top: parent.top
            topMargin: 10
            left: recordRect.right
            leftMargin: 10
            right: parent.right
            rightMargin: 10
            bottom: parent.bottom
        }

        Component {
            id: noneCmp
            Rectangle {
                anchors.fill: parent
                color: Script.defaultUnhoveredColor()
            }
        }

        Component {
            id: newGroupCmp
            Rectangle {
                anchors.fill: parent
                color: Script.defaultUnhoveredColor()
                Text {
                    id: cmpText
                    height: 45
                    font {
                        family: Script.menuTextFontFamily()
                        pixelSize: 17
                    }
                    color: Script.menuTextColor()
                    text: qsTr("Новая корневая группа:")
                    anchors {
                        top: parent.top
                        horizontalCenter: parent.horizontalCenter
                    }
                    verticalAlignment: Text.AlignVCenter
                }
                Rectangle {
                    id: groupSeparator
                    color: "black"
                    height: 2
                    width: parent.width * 0.8
                    anchors {
                        top: cmpText.bottom
                        horizontalCenter: parent.horizontalCenter
                    }
                }
                FramedButton {
                    colorScheme: Enums.RedColorScheme
                    y: 45 / 2 - height / 2
                    anchors {
                        left: parent.left
                        leftMargin: 15
                    }
                    onClicked: { loader.sourceComponent = noneCmp; }
                }
                LineEditor {
                    anchors {
                        top: groupSeparator.bottom
                        topMargin: 30
                        left: parent.left
                        leftMargin: 15
                        right: parent.right
                        rightMargin: 15
                    }
                    placeholderText: qsTr("Новое название")
                    editEnabled: true
                    validator: RegExpValidator { regExp: /[\w| ]+/ }
                    onTextChanged: {
                        let isProfit = groupRect.isProfit();
                        let exists = CoreAPI.purchaseGroupExists(editorData,
                                                                 "",
                                                                 isProfit);
                        setError(exists);
                    }
                    onAccepted: {
                        let isProfit = groupRect.isProfit();
                        CoreAPI.addPurchaseGroup(editorData,
                                                 0,
                                                 isProfit);
                        interactiveRect.setMode(Enums.None);
                    }
                    onEscaped: {
                        interactiveRect.setMode(Enums.None);
                    }
                }
            }
        }

        Loader {
            id: loader
            anchors.fill: parent
            sourceComponent: noneCmp
        }
    }
}


