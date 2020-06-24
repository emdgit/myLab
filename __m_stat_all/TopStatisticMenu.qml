import QtQuick 2.0

/// Верхнее выдвижное меню с формы статистик.
Item {

    id: menu

    /// Эмитируется при нажатии на кнопку меню
    /// Передает её порядковый номер
    signal buttonClicked ( int number )

    QtObject {
        id: d
        property int _wid: 30   // Ширина
    }

    Row {
        Rectangle {
            width: d._wid
            height: d._wid
            color: "red"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked(0)
                }
            }
        }
        Rectangle {
            width: d._wid
            height: d._wid
            color: "green"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    buttonClicked(1)
                }
            }
        }
    }
}
