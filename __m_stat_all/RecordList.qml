import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

import "Common.js" as Script

Item {

    id: topItem

    function setGroupId(id) {
        if (listView.groupId === id) {
            return;
        }

        ModelManager.recordByGroupModel.setGroupId(id);
        let rows = ModelManager.recordByGroupModel.rowCount();

        if (rows === listView.model) {
            listView.model = 0;
        }

        listView.model = rows;
    }

    QtObject {
        id: _d

        readonly property int lineH: 45
        readonly property int textFontSize: 17

        readonly property string hoveredLineColor: Script.hoveredColor()
        readonly property string defaultLineColor: Script.defaultUnhoveredColor()
        readonly property string defaultTextColor: Script.menuTextColor()
        readonly property string textFontFamily: Script.menuTextFontFamily()
    }

    ScrollView {
        id: scrollView
        anchors.fill: parent

        style: ScrollViewStyle {
            id: scrollStyle

            frame: Rectangle {
                border.color: _d.defaultLineColor
                color: _d.defaultLineColor
            }

            scrollBarBackground: Rectangle {
                color: _d.defaultLineColor
                implicitWidth: 12
            }

            /// Ползунок скролла
            handle: Rectangle {
                id: scrollHandle
                color: _d.hoveredLineColor
                implicitWidth: 10
                implicitHeight: 50
                radius: 5
                border.color: "black"
                border.width: 1.2
                antialiasing: true
            }

            /// Кнопка скролла "вниз"
            incrementControl: Rectangle {
                width: 12
                height: 0
            }

            /// Кнопка скролла "вверх"
            decrementControl: Rectangle {
                width: 12
                height: 0
            }
        }

        ListView {

            property int groupId: -1

            id: listView

            anchors.fill: parent

            model: 0
            clip: true

            function record(row) {
                return ModelManager.recordByGroupModel.recordName(row);
            }

            delegate: Rectangle {
                id: dlg
                height: _d.lineH
                width: listView.width
                color: _d.defaultLineColor
                Text {
                    id: recordText
                    anchors {
                        fill: parent
                        leftMargin: 15
                    }
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignLeft
                    color: _d.defaultTextColor
                    font {
                        family: _d.textFontFamily
                        pixelSize: _d.textFontSize
                    }
                    text: qsTr(listView.record(index))
                }
            }
        }
    }
}
