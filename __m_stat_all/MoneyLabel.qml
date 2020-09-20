import QtQuick 2.0

import "Common.js" as Script

// "Виджет", отображающий некоторую сумму в рублях
Item {

    id: topItem

    property alias summ: label.summ
    property alias meaning: label.meaning

    height: parent.height

    Text {
        id: label

        // Сумма, которую нужно отобразить
        property real summ: 0.0

        // Что означает сумма. Пояснение перед ней.
        property string meaning: ""

        QtObject {
            id: _d
            property string symbol: "₽"
        }

        anchors.top: parent.top
        anchors.bottom: parent.bottom

        verticalAlignment: Text.AlignVCenter

        text: summInRub()
        color: Script.menuTextColor()
        font.family: Script.menuTextFontFamily()
        font.pixelSize: Script.menuTextFontSize() - 2

        onSummChanged: {
            text = summInRub();
        }

        onMeaningChanged: {
            text = summInRub();
        }

        function summInRub() {
            if ( meaning === "" ) {
                return summ + _d.symbol;
            } else {
                return meaning + ": " + summ + _d.symbol;
            }
        }

        function setSymbol(symbol) { _d.symbol = symbol; }
    }

    function setMoneyMode() { label.setSymbol( "₽" ); }
    function setPercentMode() { label.setSymbol( "%" ); }
}

