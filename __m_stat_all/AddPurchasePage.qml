import QtQuick 2.0
import QtQuick.Controls 1.4

/// Страница добавления данных на форме статистик
Item {

    SlideEditorPopup {
        id: textEditor
        z: 100
        anchors.horizontalCenter: parent.horizontalCenter
        placeholderText: qsTr("Запись...")
    }

    Calendar {
        z: 90
        id: calendar
        anchors.centerIn: parent
        locale:  Qt.locale("ru_ru")
    }

}
