/*
 * top to bottom
 * backward
 */

for (y = 0; y < image.height; y++) {
    for (x = image.width - 1; x >= 0; x--) {
        image.addPoint(x, y);
    }
}
