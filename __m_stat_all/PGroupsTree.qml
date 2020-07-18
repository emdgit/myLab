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

    /// Сигнал эмитируется, когда была нажата кнопка "Выбрать"
    /// Передает id выбранной группы
    signal accept( int groupId )

    QtObject {
        id: _d
        /// Вертикальное расстояноие под нижние кнопки
        readonly property int buttonsH: 40
        readonly property int controlRectMargin: 6

        property bool _setup : setup()

        function setup() {
            ModelManager.spendModel.setRootName(qsTr("Корневая группа"));
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
                id: branchIcon
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
        selection: ItemSelectionModel {
            id: selector
            model: ModelManager.spendModel
        }
        onClicked: { lastIndex = index; }

        function resetLastIndex() { lastIndex = 0; }
    }

    Behavior on opacity {
        NumberAnimation {
            duration: 300
        }
    }

    SwipeView {
        id: controlSwipeView
        interactive: false
        currentIndex: 0
        clip: true

        anchors {
            top: treeView.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }

        Item {
            Rectangle {

                id: controlRect
                anchors.fill: parent

                RowLayout {
                    anchors.fill: parent
                    anchors.rightMargin: _d.controlRectMargin

                    PicButton {
                        id: hideButton

                        defaultPic: "qrc:/img/images/arrow_right_square.svg"
                        hoverPic: "qrc:/img/images/arrow_right_square_hover.svg"
                        unactivePic: "qrc:/img/images/arrow_right_square_unactive.svg"

                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                        Layout.margins: _d.controlRectMargin


                        onClicked: { topItem.opacity = 0 }
                    }

                    PicButton {
                        id: newGroupButton

                        defaultPic: "qrc:/img/images/folder_plus.svg"
                        hoverPic: "qrc:/img/images/folder_plus_hover.svg"
                        unactivePic: "qrc:/img/images/folder_plus_unactive.svg"

                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        onClicked: { controlSwipeView.swipeTo(1); }
                    }

                    PicButton {
                        id: acceptButton

                        defaultPic: "qrc:/img/images/check.svg"
                        hoverPic: "qrc:/img/images/check_hover.svg"
                        unactivePic: "qrc:/img/images/check_unactive.svg"

                        Layout.alignment: Qt.AlignVCenter | Qt.AlignRight

                        onClicked: {

                            var index = treeView.lastIndex;

                            if ( index === 0 ) {
                                return;
                            }

                            var groupId = treeView.model.groupId(index);
                            topItem.accept( groupId );
                            topItem.opacity = 0;
                        }
                    }

                    Rectangle { Layout.fillWidth: true }
                }
            }
        }
        Item {
            SlideEditor {
                text: qsTr( "Имя:" )
                alternativePlaceholder: qsTr("Имя новой группы..")
                onAccepted: {
                    var index = treeView.lastIndex;

                    if ( index !== 0 ) {
                        var groupId = ModelManager.spendModel.groupId(index)
                        CoreAPI.addPurchaseGroup(editorText, groupId, topItem.profit);
                    }

                    controlSwipeView.swipeTo(0);
                }
                onEscaped: {
                    controlSwipeView.swipeTo(0);
                }
            }
        }

        /// Вертит SwipeView
        function swipeTo( index ) {

            if ( index < 0 || index > 1 ) {
                return;
            }

            if ( index === 0 ) {
                controlSwipeView.currentIndex = 0;
                ModelManager.spendModel.setShowRoot(false);
            }
            else {
                controlSwipeView.currentIndex = 1
                ModelManager.spendModel.setShowRoot(true)
            }

            treeView.resetLastIndex();
            currentIndex = index;
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
