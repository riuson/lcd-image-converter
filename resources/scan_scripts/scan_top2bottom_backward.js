/*
 * top to bottom
 * backward
 */

for (var y = 0; y < image.height; y++) {
    for (var x = image.width - 1; x >= 0; x--) {
        image.addPoint(x, y);
    }
}
