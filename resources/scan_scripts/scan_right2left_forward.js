/*
 * right to left
 * forward
 */

for (var x = image.width - 1; x >= 0; x--) {
    for (var y = 0; y < image.height; y++) {
        image.addPoint(x, y);
    }
}
