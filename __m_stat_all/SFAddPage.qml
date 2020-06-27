import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0 // 1.12 Win

/// Страница добавления данных на форме статистик
/// Statistic Form Add Page
Item {

    id: topItem

    MouseArea {
        anchors.fill: parent
        onClicked: {
            recordEditor.forceActiveFocus()
            summEditor.forceActiveFocus()
            summEditor.forceActiveFocus()
        }
    }

    FocusScope{
        id: fScope
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        Frame {
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            ColumnLayout {
                SlideEditorPopup{
                    id: recordEditor
                    internalId: 0
                    text: qsTr("Запись")
                    onActivated: { switchSlideEditor(number) }
                }
                SlideEditorPopup{
                    id: summEditor
                    internalId: 1
                    text: qsTr("Сумма")
                    onActivated: { switchSlideEditor(number) }
                }
                SlideEditorPopup{
                    id: dateEditor
                    internalId: 2
                    text: qsTr("Дата")
                    onActivated: { switchSlideEditor(number) }
                }
            }
        }
    }

    /// Обработчик сигналов активации виджетов "SlideEditor",
    /// устанавливает активному эдитору повышенное значение z
    function switchSlideEditor( activeEditor ) {
        var topZ = 42   // Потому что.
        var botZ = 0

        function setZ( recZ, sumZ, dateZ ) {
            recordEditor.z = recZ
            summEditor.z = sumZ
            dateEditor.z = dateZ
        }

        switch ( activeEditor ) {
            case 0:
                setZ( topZ, botZ, botZ )
                break
            case 1:
                setZ( botZ, topZ, botZ )
                break
            case 2:
                setZ( botZ, botZ, topZ )
                break
        }
    }
}
