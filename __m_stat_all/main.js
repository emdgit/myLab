var component = Qt.createComponent( "Block.qml" )

var maxBlocksCount = 80

var blocksArray = new Array;

function createNewBlock(x, y)
{
    if(component.status !== Component.Ready)
    {
        return false;
    }

    if(blocksArray.length > maxBlocksCount)
    {
        removeAllBlocks();
    }

    var newBlock = component.createObject(canvas);

    if(newBlock === null)
    {
        return false;
    }

    var iconFile = mObject.randomIcon

    newBlock.source = ( /*iconFile*/"exit.png" );

    newBlock.x = x;
    newBlock.y = y;

    newBlock.show = true;

    blocksArray.push(newBlock);

    return true;
}

function handleKey()
{
    var x = Math.floor( Math.random() * canvas.width )
    var y = Math.floor( Math.random() * canvas.height )

    createNewBlock( x, y )
}

function removeAllBlocks()
{
    for(var i = 0; i < blocksArray.length; ++i)
    {
        blocksArray[i].remove = true;
    }

    while(blocksArray.length != 0)
    {
        blocksArray.pop();
    }
}
