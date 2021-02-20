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

    //property alias model: treeView.model

    QtObject {
        id: _d
        readonly property int controlRectMargin: 6
        readonly property string rootName: qsTr("Корневая группа")

        property bool _setup : setup()

        function setup() {
            ModelManager.spendModel.setRootName(rootName);
            ModelManager.profitModel.setRootName(rootName);

            ModelManager.spendModel.setShowRoot(rootName);
            ModelManager.profitModel.setShowRoot(rootName);
            return true;
        }
    }

    TreeView {
        id: treeView

        anchors.fill: parent

        /// Последний кликнутый индекс.
        /// (Строка может давать warning, игнорировать, тут НУЖЕН var)
        default property var lastIndex: 0

        /// "Высота" строки
        readonly property int rowHeight: 45

        model: ModelManager.spendModel
        clip: false
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff

        style: TreeViewStyle {

            id: tvStyle

            backgroundColor: "transparent"
            alternateBackgroundColor: "transparent"
            indentation: 9

            frame: Rectangle {
                border.color: "transparent"
                color: "transparent"
            }

            /*!
             *  Иконка открыто / закрыто (из коробки).
             *  Стандартная реализация "branchDelegate" не может
             *  превышать размеры "indentation". Потому нужно обеспечить
             *  его неактивность.
             */
            branchDelegate: Item {}

            rowDelegate: Rectangle {
                id: rowDlg
                height: treeView.rowHeight
                color: Script.defaultUnhoveredColor()
            }

            itemDelegate: Rectangle {

                id: itemDlg

                readonly property int _id: Script.getUniqueNumber()

                color: tst()

                /// Иконка раскрыто / схлопнуто
                Rectangle {

                    property string branchIconDefault: "qrc:/img/images/folder_opened.svg"
                    property string branchIconHovered: "qrc:/img/images/folder_opened_hover.svg"
                    property bool isHovered: false

                    readonly property bool hasChildren: styleData.hasChildren

                    id: icon
                    height: 30
                    width: 30
                    anchors.verticalCenter: parent.verticalCenter
                    color: Script.defaultUnhoveredColor()

                    Component {
                        id: branchIcon
                        Image {
                            id: name
                            source: icon.branchIconDefault
                            sourceSize.width: width
                            sourceSize.height: height
                            anchors.centerIn: parent
                        }
                    }

                    Loader {
                        anchors.fill: parent
                        sourceComponent: icon.hasChildren ? branchIcon
                                                          : undefined
                    }
                }

                Text {
                    anchors {
                        left: icon.right
                        leftMargin: 13
                        top: parent.top
                        bottom: parent.bottom
                    }

                    verticalAlignment: Text.AlignVCenter
                    text: styleData.value
                    color: Script.menuTextColor()
                    font.family: Script.menuTextFontFamily()
                    font.pixelSize: 17
                }

                MouseArea {
                    id: mArea
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                    hoverEnabled: true
                    onEntered: {
                        hoverEngine.number = itemDlg._id;
                    }
                    onMouseXChanged: {
                        hoverEngine.number = itemDlg._id;
                    }

                    onClicked: {
//                        let button = mouse.button;
//                        if (button === Qt.LeftButton) {
//                            let ind = styleData.index;
//                            let cmd = ItemSelectionModel.Select;
//                            selector.setCurrentIndex(ind, cmd)
//                        } else if (button === Qt.RightButton) {
//                            selector.clearCurrentIndex();
//                        }
                    }
                    onDoubleClicked: {
                        let index = styleData.index;
                        if (treeView.model.rowCount(index) === 0) {
                            return;
                        }
                        if (styleData.isExpanded) {
                            treeView.collapse(index);
                        } else {
                            treeView.expand(index);
                        }
                    }
                }
                function tst() {
                    let s = styleData.selected;
                    if (s) {
                        return "red";
                    } else if (itemDlg._id === hoverEngine.number) {
                        return "blue";
                    } else {
                        return Script.defaultUnhoveredColor();
                    }
                }
                Component.onCompleted: {
                }
            }

            scrollBarBackground: Rectangle {
                color: Script.defaultUnhoveredColor()
                implicitWidth: 12
            }

            /// Ползунок скролла
            handle: Rectangle {
                id: scrollHandle
                color: Script.hoveredColor()
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
            }
        }

        headerVisible: false

        TableViewColumn {
            width: treeView.width
            role: "r_pgroup_name"
        }

        selection: ItemSelectionModel {
            id: selector
            model: treeView.model

        }

        QtObject {
            id: hoverEngine
            property int number: -1
        }

        onClicked: { lastIndex = index; }

        onModelChanged: {
            var showRoot = false;
            treeView.model.setShowRoot(showRoot);
            if ( treeView.model.rootName !== _d.rootName ) {
                treeView.model.setRootName( _d.rootName );
            }
        }

        function resetLastIndex() { lastIndex = 0; }
    }
}
