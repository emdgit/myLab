var component;
var textRect;
var nextName;

var counter = 1;

var textRectArr = new Array;

function finishTextRect() {

    if ( component.status === Component.Ready ) {
        textRect = component.createObject( topLabel );
        textRect.show = true;
        textRect.text = nextName;

        textRectArr.push( textRect );
    }
    else {
        console.log( "Component textRect isn't ready.." );
    }
}

function createNewTextRect( name ) {
    component = Qt.createComponent( "TextRect.qml" );
    nextName = name;

    if ( component.status === Component.Ready ) {
        finishTextRect();
    }
    else {
        console.log( "Component textRect isn't ready yet" );
        component.statusChanged.connect( finishTextRect );
    }
}

function removeOldTextRects() {

    for ( var i = 0; i < textRectArr.length; ++i ) {
        textRectArr[i].hide = true;
    }

    while ( textRectArr.length !== 0 ) {
        textRectArr.pop();
    }
}

function getUniqueNumber() {
    let tmp = counter;
    counter++;
    return tmp;
}

/// Фон верхней надписи с названием текущей формы
function topLabelBackgroundColor() {
    return "#120C2A";
}

/// Цвет кнопки левого меню при её выделении
function hoveredColor() {
    return "#3A2E5C";
}

/// Дефолтный цвет левой панели меню
function defaultUnhoveredColor() {
    return "#2A2440";
}

/// Цвет фона элементов выпадающего меню подсказок при выделении. (про Записи)
function hintHoveredColor() {
    return hoveredColor();
}

/// Дефолтный цвет фона элементов выпадающего меню подсказок. (про Записи)
function hintUnhoveredColor() {
    return "white";
}

/// Цвет текста в выпадающем меню подсказок при выделении. (про Записи)
function hintTextColorHovered() {
    return menuTextColor();
}

/// Дефолтный цвет текста в выпадающем меню подсказок. (про Записи)
function hintTextColorUnhovered() {
    return "black";
}

/// Цвет текста в меню
function menuTextColor() {
    return "#F2EEDC";
}

/// Шрифт текста на кнопках в левом меню
function menuTextFontFamily() {
    return "Franklin Gothic Medium Cond";
}

/// Размер текста на кнопках в левом меню
function menuTextFontSize() {
    return 22;
}

/// Дикий оранжевый цвет
function orangeColor() {
    return "#FB8202";
}
