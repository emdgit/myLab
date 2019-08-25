var component
var textRect
var nextName

var textRectArr = new Array

function createNewTextRect( name ) {
    component = Qt.createComponent( "TextRect.qml" )
    nextName = name

    if ( component.status === Component.Ready ) {
        finishTextRect()
    }
    else {
        console.log( "Component textRect isn't ready yet" )
        component.statusChanged.connect( finishTextRect )
    }
}

function finishTextRect() {

    if ( component.status === Component.Ready ) {
        textRect = component.createObject( topLabel )
        textRect.show = true
        textRect.text = nextName

        textRectArr.push( textRect )
    }
    else {
        console.log( "Component textRect isn't ready.." )
    }
}

function removeOldTextRects() {

    for ( var i = 0; i < textRectArr.length; ++i ) {
        textRectArr[i].hide = true;
    }

    while ( textRectArr.length !== 0 ) {
        textRectArr.pop()
    }
}
