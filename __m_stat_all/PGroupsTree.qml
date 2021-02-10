import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import QtQml.Models 2.11

import OwlComponents 1.0

import "Common.js" as Script

/// Элемент для работы с деревом групп покупок
Item {

    id: topItem

    width: 300
    opacity: 0

    property bool profit: false

    property alias model: treeView.model

    QtObject {
        id: _d
        readonly property int controlRectMargin: 6
        readonly property string rootName: qsTr("Корневая группа")

        property bool _setup : setup()

        function setup() {
            ModelManager.spendModel.setRootName(rootName);
            ModelManager.profitModel.setRootName(rootName);
            return true;
        }
    }

    TreeView {
        id: treeView

        /// Последний кликнутый индекс.
        /// (Строка может давать warning, игнорировать, тут НУЖЕН var)
        default property var lastIndex: 0

        model: ModelManager.spendModel
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        anchors.fill: parent

        style: TreeViewStyle {
            backgroundColor: "transparent"
            alternateBackgroundColor: "transparent"
            frame: Rectangle {
                border.color: "transparent"
            }
            branchDelegate: Item {
                id: branchIcon
                width: indentation
//                height: 16
                height: indentation
                Image {
                    id: icon
                    visible: styleData.column === 0 && styleData.hasChildren
                    source: styleData.isExpanded ? "qrc:/img/images/folder_opened.svg"
                                                 : "qrc:/img/images/folder_closed.svg"
                    anchors.centerIn: parent
//                    anchors.verticalCenterOffset: 2
                }
            }
        }

        headerDelegate: Rectangle{ height: 1 }

        TableViewColumn {
            width: treeView.width
            role: "r_pgroup_name"
        }

        selection: ItemSelectionModel {
            id: selector
            model: treeView.model
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

    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }

}
