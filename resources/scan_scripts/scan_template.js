%1

function checkProcessState()
{
    if (image.needBreakScan)
    {
        throw "scan break requested";
    }
}

function addImagePoint(pointX, pointY)
{
    checkProcessState();
    return image.addPoint(pointX, pointY);
}
