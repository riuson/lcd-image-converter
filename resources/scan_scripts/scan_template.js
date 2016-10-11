var __internal_points_counter = 0;
var __internal_image_width = image.width;
var __internal_image_height = image.height;
var __internal_points_max_count = (__internal_image_width * __internal_image_height * 120) / 100;

if (__internal_points_max_count < 100000) {
    __internal_points_max_count = 100000;
}

%1

function checkProcessState()
{
    if (__internal_points_counter > __internal_points_max_count)
    {
        image.scanTerminated = true;
        throw "scan break requested";
    }
}

function addImagePoint(pointX, pointY)
{
    __internal_points_counter++;
    checkProcessState();
    return image.addPoint(pointX, pointY);
}
