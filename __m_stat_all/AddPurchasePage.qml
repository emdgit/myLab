import QtQuick 2.0
import QtQuick.Controls 1.4

/// Страница добавления данных на форме статистик
Item {

    MouseArea {
        anchors.fill: parent
        onClicked: {
            forceActiveFocus();
        }
    }

    SpendProfitButton {
        id: profitSwitch
        anchors {
            top: parent.top
//            topMargin: 15
            left: parent.left
            leftMargin: 15
        }
    }

    RecordEditorPopup {
        id: textEditor
//        z: 100
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        placeholderText: qsTr("Запись...")
    }

    SummEditor {
        id: summEditor
        anchors {
            horizontalCenter: parent.horizontalCenter
            top: textEditor.bottom
            topMargin: 30
        }
        placeholderText: qsTr("Сумма...")
        validator: IntValidator {
            bottom: 1
            // Оптимистично, не правда ли?
            top: 2000000000
        }
    }

    Calendar {
        z: 90
        id: calendar
        anchors.centerIn: parent
        locale:  Qt.locale("ru_ru")
        onDoubleClicked: {
            console.log("Date: ", date);
            // set Date here
        }
    }

}
