import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2

Item {

    id: topItem

    TableView {

        id: table

        anchors.fill: parent

        model: ModelManager.purchaseModel

        TableViewColumn {
            role: "PName"
            title: qsTr("Запись")
        }

        TableViewColumn {
            role: "PSumm"
            title: qsTr("Сумма")
        }

        TableViewColumn {
            role: "PLastDate"
            title: qsTr("Последняя покупка")
        }
    }
}
