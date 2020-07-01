import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Styles 1.4

import "Common.js" as Script

/// Элемент для работы с деревом групп покупок
Item {

    id: topItem

    width: 300
    opacity: 0

    /// Сигнал эмитируется, когда была нажата кнопка "Выбрать"
    /// Передает id выбранной группы
    signal accept( int groupId )

    QtObject {
        id: _d
        /// Вертикальное расстояноие под нижние кнопки
        readonly property int buttonsH: 40
    }

    TreeView {
        id: treeView
        model: ModelManager.spendModel
        horizontalScrollBarPolicy: Qt.ScrollBarAlwaysOff
        height: parent.height - _d.buttonsH
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        style: TreeViewStyle {
            backgroundColor: "transparent"
            alternateBackgroundColor: "transparent"
            frame: Rectangle {
                border.color: "transparent"
            }
            branchDelegate: Item {
                width: indentation
                height: 16
                Image {
                    id: icon
                    visible: styleData.column === 0 && styleData.hasChildren
                    source: styleData.isExpanded ? "qrc:/img/images/folder_opened.svg"
                                                 : "qrc:/img/images/folder_closed.svg"
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 2
                }
            }
        }
        headerDelegate: Rectangle{ height: 1 }
        TableViewColumn {
            width: treeView.width
            role: "r_pgroup_name"
        }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }

    Rectangle {
        anchors {
            top: treeView.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        TextHoverButton {
            id: hideButton
            text: qsTr("Скрыть")
            anchors.verticalCenter: parent.verticalCenter
            x: parent.width / 4 - width / 2
            y: parent.height / 2 - height / 2
            onClicked: {
                topItem.opacity = 0
            }
        }

        TextHoverButton {
            id: acceptButton
            text: qsTr("Выбрать")
            anchors.verticalCenter: parent.verticalCenter
            x: parent.width / 2 + width / 2
            y: parent.height / 2 - height / 2
            onClicked: { topItem.onAcceptClicked() }
        }
    }

    /// Обработчик нажатия кнопки "Выбрать"
    function onAcceptClicked() {
        var index = treeView.currentIndex;
        var group_id = treeView.model.groupId( index );

        if ( group_id < 0 ) {
            return;
        }

        accept( group_id )
    }

}
