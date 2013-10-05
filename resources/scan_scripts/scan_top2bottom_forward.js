/*
 * top to bottom
 * forward
 */

for (var y = 0; y < image.height; y++) {
    for (var x = 0; x < image.width; x++) {
        image.addPoint(x, y);
    }
}
