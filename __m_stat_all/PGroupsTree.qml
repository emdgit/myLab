import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQml.Models 2.11

import QtQuick 2.15

import OwlComponents 1.0

import "Common.js" as Script

/// Элемент для работы с деревом групп покупок
Item {

    id: topItem

    width: 300

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
                color: _d.defaultLineColor
                border.width: 0
                border.color: _d.defaultLineColor
            }

        }

        ListView {

            id: listView
            anchors.fill: parent
            model: ModelManager.spendModel
            clip: true

            function hasChildren(row) {
                return model.hasUnderGroup(row);
            }

            function groupName(row) {
                return model.groupName(row);
            }

            function depth(row) {
                return model.depth(row);
            }

            function expand(row) {
                model.expand(row);
            }

            function collapse(row) {
                model.collapse(row);
            }

            function isExpanded(row) {
                return model.isExpanded(row);
            }

            delegate: Rectangle {

                property bool hovered: false

                id: dlg

                height: _d.lineH
                width: listView.width

                color: _d.defaultLineColor

                Rectangle {

                    id: selectionZone

                    anchors {
                        top: parent.top
                        left: parent.left
                        bottom: parent.bottom
                    }

                    width: 10
                    color: dlg.hovered ? _d.hoveredLineColor
                                       : _d.defaultLineColor
                }

                /// Иконка раскрыто / схлопнуто
                Rectangle {

                    readonly property bool hasChildren: listView.hasChildren(index)
                    readonly property int depthIndent: 12

                    function inIconImage(x, y) {
                        if (!hasChildren) {
                            return false;
                        }

                        let left = anchors.leftMargin + selectionZone.width;
                        let right = left + width;
                        let top = _d.lineH / 2 - height / 2;
                        let bottom = _d.lineH / 2 + height / 2;

                        return (left <= x && x <= right) &&
                               (top <= y && y <= bottom);
                    }

                    function iconDefault() {
                        if (listView.isExpanded(index)) {
                            return "qrc:/img/images/folder_opened.svg";
                        } else {
                            return "qrc:/img/images/folder_closed.svg";
                        }
                    }

                    function iconHovered() {
                        if (listView.isExpanded(index)) {
                            return "qrc:/img/images/folder_opened_hover.svg";
                        } else {
                            return "qrc:/img/images/folder_closed_hover.svg";
                        }
                    }

                    function iconSource() {
                        if (dlg.hovered) {
                            return iconHovered();
                        } else {
                            return iconDefault();
                        }
                    }

                    id: icon
                    height: 30
                    width: 30
                    color: _d.defaultLineColor

                    anchors {
                        left: selectionZone.right
                        leftMargin: 5 + depthIndent * listView.depth(index)
                        verticalCenter: parent.verticalCenter
                    }

                    Component {
                        id: branchIconComponent
                        Image {
                            Connections {
                                target: mArea
                                function onDoubleClicked(mouse) {
                                    branchIcon.source = icon.iconSource();
                                }
                            }
                            Connections {
                                target: dlg
                                function onHoveredChanged(hovered) {
                                    branchIcon.source = icon.iconSource();
                                }
                            }
                            id: branchIcon
                            source: icon.iconSource()
                            sourceSize.width: width
                            sourceSize.height: height
                            anchors.centerIn: parent
                        }
                    }

                    Loader {
                        anchors.fill: parent
                        sourceComponent: icon.hasChildren ? branchIconComponent
                                                          : undefined
                    }
                }

                Text {
                    function textBold() {
                        return dlg.hovered ? true : false;
                    }

                    Connections {
                        target: dlg
                        function onHoveredChanged(hovered) {
                            text.font.bold = text.textBold();
                        }
                    }

                    id: text

                    anchors {
                        left: icon.right
                        leftMargin: 13
                        top: parent.top
                        bottom: parent.bottom
                    }

                    verticalAlignment: Text.AlignVCenter
                    text: listView.groupName(index)
                    color: _d.defaultTextColor
                    font.family: _d.textFontFamily
                    font.pixelSize: _d.textFontSize
                }

                MouseArea {
                    id: mArea

                    function expandOrCollapse() {
                        let expanded = listView.isExpanded(index);

                        if (!expanded) {
                            listView.expand(index);
                        } else {
                            listView.collapse(index);
                        }
                    }

                    hoverEnabled: true
                    anchors.fill: parent
                    onEntered: { dlg.hovered = true; }
                    onExited: { dlg.hovered = false; }
                    onClicked: {
                        if (icon.inIconImage(mouseX, mouseY)) {
                            expandOrCollapse();
                        }
                    }
                    onDoubleClicked: {
                        expandOrCollapse();
                    }
                }
            }
        }
    }
}
