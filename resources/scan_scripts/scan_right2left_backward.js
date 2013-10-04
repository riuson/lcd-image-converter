/*
 * right to left
 * backward
 */

for (x = image.width - 1; x >= 0; x--) {
    for (y = image.height - 1; y >= 0; y--) {
        image.addPoint(x, y);
    }
}
