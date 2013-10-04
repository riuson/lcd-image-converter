/*
 * right to left
 * forward
 */

for (x = image.width - 1; x >= 0; x--) {
    for (y = 0; y < image.height - 1; y++) {
        image.addPoint(x, y);
    }
}
