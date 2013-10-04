/*
 * left to right
 * backward
 */

for (x = 0; x < image.width - 1; x++) {
    for (y = image.height - 1; y >= 0; y--) {
        image.addPoint(x, y);
    }
}
